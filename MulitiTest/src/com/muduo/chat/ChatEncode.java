package com.muduo.chat;
import java.util.zip.Adler32;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.MessageToByteEncoder;
import io.netty.util.CharsetUtil;


public class ChatEncode extends MessageToByteEncoder<ChatMessage>{
		
	@Override
	protected void encode(ChannelHandlerContext ctx, ChatMessage msg,
			ByteBuf out) throws Exception {
		int nameLen = msg.getType().length();
		int byte_size = msg.getMessage().getSerializedSize();
		
		int len = 4 + nameLen + byte_size + 4;
		
		out.writeByte(len);
		out.writeByte(nameLen);
		out.writeBytes(msg.getType().getBytes(CharsetUtil.UTF_8));
		out.writeBytes(msg.getMessage().toByteArray());
		Adler32 check = new Adler32();
		check.update(out.array(),4,out.readableBytes()-4);
		int checkSum = (int) check.getValue();
		out.writeLong(checkSum);
		
	}
	
}
