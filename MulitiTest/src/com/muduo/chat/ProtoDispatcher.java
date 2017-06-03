package com.muduo.chat;
import io.netty.channel.ChannelHandlerContext;

import java.util.HashMap;
import java.util.Map;

import com.google.protobuf.Descriptors.Descriptor;
import com.google.protobuf.Message;


public class ProtoDispatcher {
	
	private Map<Descriptor, HandMessage> callbacks = new HashMap<Descriptor, HandMessage>();
	
	public void onProtobufMessage(ChannelHandlerContext ctx, Message msg){
		HandMessage handle = callbacks.get(msg.getDescriptorForType());
		handle.onMessage(ctx, msg);
	}
	
	public void registerMessageCallback(Descriptor desc,HandMessage callback){
		callbacks.put(desc, callback);
	}
}
