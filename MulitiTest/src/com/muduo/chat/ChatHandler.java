package com.muduo.chat;
import java.util.zip.Adler32;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.Message;
import com.muduo.proto.ChatProtos;
import com.muduo.proto.GroupProtos;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;


public class ChatHandler extends ChannelInboundHandlerAdapter{
	
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
		while(buf.readableBytes() >= kMinMessageLen+kHeaderLen){
			int len = buf.readInt();
			if (len > kMaxMessageLen || len < kMinMessageLen){
				//error call back.
			}
			else if (buf.readableBytes() >= (len)){
				int error = -1;
				Message message = parse(buf,len);
				if (message != null){
					dispatcher.onProtobufMessage(ctx, message);
				}
			}
		}
	
	}
	
	public Message parse(ByteBuf buf,int len){
		Message message = null;
		int expectedCheckSUm = buf.getInt(len);
		Adler32 check = new Adler32();
		check.update(buf.array(), buf.readerIndex(), len-kHeaderLen);
		int checkSum = (int) check.getValue();
		if (checkSum == expectedCheckSUm){
			int nameLen = buf.readInt();
			if (nameLen >= 2 && nameLen <= len - 2*kHeaderLen){
				byte[] nameByte = new byte[nameLen];
				buf.readBytes(nameByte, 0, nameLen);
				String typeName = new String(nameByte);
				message = createMessage(typeName,buf.array(),buf.readerIndex(),buf.readableBytes()-4);
			}
		}
		return message;
	}
	
	public Message createMessage(String typename,byte[] array,int offset,int length) {
		Message message = null;
		try {
			if (typename.equals("chat.Connect")){
				return ChatProtos.Connect.getDefaultInstance().getParserForType().parseFrom(array,offset,length);
			}else if (typename.equals("chat.ChatMessage")){
				return ChatProtos.ChatMessage.getDefaultInstance().getParserForType().parseFrom(array,offset,length);
			}else if (typename.equals("chat.heart")){
				return ChatProtos.heart.getDefaultInstance().getParserForType().parseFrom(array,offset,length);
			}else if (typename.equals("group.GroupMessage")){
				return GroupProtos.GroupMessage.getDefaultInstance().getParserForType().parseFrom(array,offset,length);
			}else if (typename.equals("group.HandleGroup")){
				return GroupProtos.HandleGroup.getDefaultInstance().getParserForType().parseFrom(array,offset,length);
			}
		} catch (InvalidProtocolBufferException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return message;
	}
}
