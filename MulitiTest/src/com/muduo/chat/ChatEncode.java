package com.muduo.chat;
import java.util.zip.Adler32;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.MessageToByteEncoder;
import io.netty.util.CharsetUtil;


public class ChatEncode extends MessageToByteEncoder<ChatMessage>{
	private static final Logger logger = LoggerFactory.getLogger("ChatEncode");

	@Override
	protected void encode(ChannelHandlerContext ctx, ChatMessage msg,
			ByteBuf out) throws Exception {
		int nameLen = msg.getType().length();
		int byte_size = msg.getMessage().toByteArray().length;
		
		int len = 4 + nameLen + byte_size + 4;
		
		out.writeInt(len);
		out.writeInt(nameLen);
		out.writeBytes(msg.getType().getBytes(CharsetUtil.UTF_8));
		out.writeBytes(msg.getMessage().toByteArray());
		Adler32 check = new Adler32();
		byte[] dst = new byte[out.readableBytes()-4];
		out.getBytes(4, dst, 0, out.readableBytes()-4);
		check.update(dst,0,out.readableBytes()-4);
		int checkSum = (int) check.getValue();
		out.writeInt(checkSum);
		
	}
	
}
