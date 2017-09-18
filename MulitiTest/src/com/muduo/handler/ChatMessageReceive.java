package com.muduo.handler;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.netty.channel.ChannelHandlerContext;

import com.muduo.chat.HandMessage;
import com.muduo.proto.ChatProtos;

public class ChatMessageReceive implements HandMessage{

	private static final Logger logger = LoggerFactory.getLogger("ChatMessageReceive");
	private ChatManager manager;
	
	public ChatMessageReceive(ChatManager manager) {
		this.manager = manager;
	}

	
	@Override
	public void onMessage(ChannelHandlerContext ctx, Object msg) {
		logger.info("收到了聊天消息");
		ChatProtos.ChatMessage cMessage = (ChatProtos.ChatMessage)msg;
		int from = cMessage.getFromid();
		int to = cMessage.getToid();
		String message = cMessage.getMessage();
		logger.info(from+":"+to+" "+message);
		if (manager.receiveMess(cMessage)){
			System.out.println(from+":"+to+" "+message);
		}
	}
}
