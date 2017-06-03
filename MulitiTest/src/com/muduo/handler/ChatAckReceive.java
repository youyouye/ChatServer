package com.muduo.handler;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.netty.channel.ChannelHandlerContext;

import com.muduo.chat.HandMessage;
import com.muduo.proto.ChatProtos;
import com.muduo.proto.ChatProtos.ChatAck;

public class ChatAckReceive implements HandMessage{
	
	private static final Logger logger = LoggerFactory.getLogger("ChatAckReceive");
	private ChatManager manager;
	
	public ChatAckReceive(ChatManager manager) {
		this.manager = manager;
	}
	@Override
	public void onMessage(ChannelHandlerContext ctx, Object msg) {
		logger.info("收到了chatAck");
		ChatProtos.ChatAck cMessage = (ChatAck) msg;
		int fromid = cMessage.getFromid();
		int toid = cMessage.getToid();
		int rank = cMessage.getId();
		manager.receiveAck(fromid, toid, rank);
	}

}
