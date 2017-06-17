package com.muduo.handler;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.netty.channel.ChannelHandlerContext;

import com.muduo.chat.HandMessage;
import com.muduo.proto.ChatProtos;
import com.muduo.proto.ChatProtos.ChatAck;
import com.muduo.proto.ChatProtos.OffMsgRly;

public class OffMsgSingleReceive implements HandMessage{

	private static final Logger logger = LoggerFactory.getLogger("OffMsgSingleReceive");
	private ChatManager manager;
	
	public OffMsgSingleReceive(ChatManager cm){
		this.manager = cm;
	}
	
	@Override
	public void onMessage(ChannelHandlerContext ctx, Object msg) {
		logger.info("收到了OffMsgSingleRly:");
		ChatProtos.OffMsgRly cMessage = (OffMsgRly) msg;
		int page = cMessage.getPage();
		for (ChatProtos.ChatMessage cm : cMessage.getMessagesList()){
			System.out.println(cm.getMessage());
		}
		manager.receiveOffRly(page);
	}
	
}
