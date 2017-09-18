package com.muduo.chat;
import io.netty.channel.ChannelHandlerContext;


public interface HandMessage {
	public void onMessage(ChannelHandlerContext ctx, Object msg);
}
