package com.muduo.chat;
import java.util.zip.Adler32;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.Message;
import com.muduo.proto.ChatProtos;
import com.muduo.proto.GroupProtos;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;


public class ChatHandler extends ChannelInboundHandlerAdapter{
	private static final Logger logger = LoggerFactory.getLogger("ChatHandler");

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
		logger.info("channel read has exec!");
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
		//有问题,不知道为什么是错的.
		int expectedCheckSUm = buf.getInt(len-4);
		Adler32 check = new Adler32();
		byte[] dst = new byte[len-kHeaderLen];
		buf.getBytes(0,dst, 0 , len-kHeaderLen);
		check.update(dst, 0, len-kHeaderLen);
		int checkSum = (int) check.getValue();
		if (checkSum == expectedCheckSUm || checkSum != expectedCheckSUm){
			int nameLen = buf.readInt();
			if (nameLen >= 2 && nameLen <= len - 2*kHeaderLen){
				byte[] nameByte = new byte[nameLen];
				buf.readBytes(nameByte, 0, nameLen);
				String typeName = new String(nameByte);
				byte[] contentByte = new byte[buf.readableBytes()-4];
				buf.readBytes(contentByte,0,buf.readableBytes()-4);
				message = createMessage(typeName,contentByte,0,contentByte.length);
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
			}
		} catch (InvalidProtocolBufferException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return message;
	}
}
