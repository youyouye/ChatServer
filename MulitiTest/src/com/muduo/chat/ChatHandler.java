package com.muduo.chat;
import java.util.zip.Adler32;





import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.Message;
import com.muduo.proto.ChatProtos;
import com.muduo.proto.GroupProtos;

import io.netty.buffer.ByteBuf;
import io.netty.buffer.ByteBufAllocator;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;


public class ChatHandler extends ChannelInboundHandlerAdapter{
	private static final Logger logger = LoggerFactory.getLogger("ChatHandler");

    public static final Cumulator MERGE_CUMULATOR = new Cumulator() {
        @Override
        public ByteBuf cumulate(ByteBufAllocator alloc, ByteBuf cumulation, ByteBuf in) {
            ByteBuf buffer;
            if (cumulation.writerIndex() > cumulation.maxCapacity() - in.readableBytes()
                    || cumulation.refCnt() > 1) {
                // Expand cumulation (by replace it) when either there is not more room in the buffer
                // or if the refCnt is greater then 1 which may happen when the user use slice().retain() or
                // duplicate().retain().
                //
                // See:
                // - https://github.com/netty/netty/issues/2327
                // - https://github.com/netty/netty/issues/1764
                buffer = expandCumulation(alloc, cumulation, in.readableBytes());
            } else {
                buffer = cumulation;
            }
            buffer.writeBytes(in);
            in.release();
            return buffer;
        }

    };
    ByteBuf cumulation;
    private Cumulator cumulator = MERGE_CUMULATOR;
    private int numReads;
    private boolean first;

	private final int kHeaderLen = 4;
	private final int kMinMessageLen = 2*kHeaderLen + 2;
	private final int kMaxMessageLen = 64*1024*1024;
	
	private ProtoDispatcher dispatcher;
	
	public ChatHandler(ProtoDispatcher dispatcher) {
		super();
		this.dispatcher = dispatcher;
	}
	
	@Override
	public void channelRead(ChannelHandlerContext ctx, Object msg){
        ByteBuf buf = (ByteBuf) msg;
		logger.info("channel read has exec!"+buf.readableBytes());
		
        first = (cumulation == null);
        if (first) {
            cumulation = buf;
        } else {
            cumulation = cumulator.cumulate(ctx.alloc(), cumulation, buf);
        }
		
		while(cumulation.readableBytes() >= kMinMessageLen+kHeaderLen){
			byte[] dst = new byte[4];
			cumulation.getBytes(cumulation.readerIndex(),dst, 0 , 4);
		//	int len = ByteBuffer.wrap(dst).order(ByteOrder.BIG_ENDIAN).getInt();
			int i = cumulation.readerIndex();
			int len = cumulation.getInt(cumulation.readerIndex());
			if (len > kMaxMessageLen || len < kMinMessageLen){
				//error call back.
				logger.info("出问题??"+len);
				break;
			}
			else if (cumulation.readableBytes() >= (len + 4)){
				int test = cumulation.readableBytes();
				int error = -1;
				Message message = parse(cumulation,len);
				if (message != null){
					dispatcher.onProtobufMessage(ctx, message);
				}else{
					break;
				}
			}else {
				break; //噢,刚才少了这句,实际上会无限循环下去..
			}
		}
		if (cumulation.isReadable()){
			byte[] res = new byte[cumulation.readableBytes()];
			cumulation.getBytes(cumulation.readerIndex(), res,0,cumulation.readableBytes());
			cumulation.clear();		
			cumulation.writeBytes(res);
		}else{
			cumulation.clear();
		}
	}
	
	public Message parse(ByteBuf buf,int len){
		Message message = null;
		//有问题,不知道为什么是错的.
		buf.readInt();
		int expectedCheckSUm = buf.getInt(len);
		Adler32 check = new Adler32();
		byte[] dst = new byte[len-kHeaderLen];
		buf.getBytes(buf.readerIndex(),dst, 0 , len-kHeaderLen);
		check.update(dst, 0, len-kHeaderLen);
		int checkSum = (int) check.getValue();
		if (checkSum == expectedCheckSUm || checkSum != expectedCheckSUm){
			
//			byte[] namedst = new byte[4];
//			buf.getBytes(4,namedst, 0 , 4);
//			int nameLen = ByteBuffer.wrap(dst).order(ByteOrder.BIG_ENDIAN).getInt();
			int nameLen = buf.readInt();
			if (nameLen >= 2 && nameLen <= len - 2*kHeaderLen){
				byte[] nameByte = new byte[nameLen];
				buf.readBytes(nameByte, 0, nameLen);
				String typeName = new String(nameByte);
				byte[] contentByte = new byte[len-8-nameLen];
				buf.readBytes(contentByte,0,len-8-nameLen);
				buf.readInt();
				message = createMessage(typeName,contentByte,0,len-8-nameLen);
			}
		}
		return message;
	}
	
	public Message createMessage(String typename,byte[] array,int offset,int length) {
		Message message = null;
		try {
			if (typename.equals("chat.Connect\0")){
				return ChatProtos.Connect.getDefaultInstance().getParserForType().parseFrom(array,offset,length);
			}else if (typename.equals("chat.ChatMessage\0")){
				return ChatProtos.ChatMessage.getDefaultInstance().getParserForType().parseFrom(array,offset,length);
			}else if (typename.equals("chat.ChatAck\0")){
				return ChatProtos.ChatAck.getDefaultInstance().getParserForType().parseFrom(array,offset,length);
			}else if (typename.equals("chat.heart\0")){
				return ChatProtos.heart.getDefaultInstance().getParserForType().parseFrom(array,offset,length);
			}else if (typename.equals("group.GroupMessage\0")){
				return GroupProtos.GroupMessage.getDefaultInstance().getParserForType().parseFrom(array,offset,length);
			}else if (typename.equals("group.HandleGroup\0")){
				return GroupProtos.HandleGroup.getDefaultInstance().getParserForType().parseFrom(array,offset,length);
			}else if (typename.equals("chat.OffMsgRly\0")){
				return ChatProtos.OffMsgRly.getDefaultInstance().getParserForType().parseFrom(array,offset,length);
			}
		} catch (InvalidProtocolBufferException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return message;
	}
	
    @Override
    public final void handlerRemoved(ChannelHandlerContext ctx) throws Exception {
        ByteBuf buf = cumulation;
        if (buf != null) {
            // Directly set this to null so we are sure we not access it in any other method here anymore.
            cumulation = null;

            int readable = buf.readableBytes();
            if (readable > 0) {
                ByteBuf bytes = buf.readBytes(readable);
                buf.release();
                ctx.fireChannelRead(bytes);
            } else {
                buf.release();
            }
            numReads = 0;
            ctx.fireChannelReadComplete();
        }
    }

	
	
	
    static ByteBuf expandCumulation(ByteBufAllocator alloc, ByteBuf cumulation, int readable) {
        ByteBuf oldCumulation = cumulation;
        cumulation = alloc.buffer(oldCumulation.readableBytes() + readable);
        cumulation.writeBytes(oldCumulation);
        oldCumulation.release();
        return cumulation;
    }
	
	//仿照着写..
    public interface Cumulator {
        ByteBuf cumulate(ByteBufAllocator alloc, ByteBuf cumulation, ByteBuf in);
    }
	
}
