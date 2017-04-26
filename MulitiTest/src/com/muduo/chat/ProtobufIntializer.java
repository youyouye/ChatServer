package com.muduo.chat;
import com.google.protobuf.MessageLite;
import com.muduo.handler.ChatMessageReceive;
import com.muduo.handler.ConnectReceive;
import com.muduo.handler.GroupHandleReceive;
import com.muduo.handler.GroupMessageReceive;
import com.muduo.handler.HeartReceive;
import com.muduo.proto.ChatProtos;
import com.muduo.proto.GroupProtos;
import com.muduo.proto.GroupProtos.GroupMessage;

import io.netty.channel.Channel;
import io.netty.channel.ChannelInitializer;
import io.netty.handler.codec.protobuf.ProtobufDecoder;
import io.netty.handler.codec.protobuf.ProtobufEncoder;
import io.netty.handler.codec.protobuf.ProtobufVarint32FrameDecoder;


public class ProtobufIntializer extends ChannelInitializer<Channel>{

	private final MessageLite lite;
	private ProtoDispatcher dispatcher;
	
	public ProtobufIntializer(MessageLite lite) {
		this.lite = lite;
		initDispatcher();
	}
	
	@Override
	protected void initChannel(Channel ch) throws Exception {
		ch.pipeline().addLast(new ChatHandler(dispatcher))
			.addLast(new ChatEncode());
	}
	
	public void initDispatcher(){
		dispatcher.registerMessageCallback(ChatProtos.Connect.getDescriptor(), new ConnectReceive());
		dispatcher.registerMessageCallback(ChatProtos.ChatMessage.getDescriptor(), new ChatMessageReceive());
		dispatcher.registerMessageCallback(ChatProtos.heart.getDescriptor(), new HeartReceive());
		dispatcher.registerMessageCallback(GroupProtos.GroupMessage.getDescriptor(), new GroupMessageReceive());
		dispatcher.registerMessageCallback(GroupProtos.HandleGroup.getDescriptor(), new GroupHandleReceive());
	}
	
}
