package com.muduo.handler;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.netty.channel.ChannelHandlerContext;

import com.muduo.chat.HandMessage;

public class HeartReceive implements HandMessage{
	private static final Logger logger = LoggerFactory.getLogger("HeartReceive");

	@Override
	public void onMessage(ChannelHandlerContext ctx, Object msg) {		
		logger.info("收到了心跳");
	}

}
