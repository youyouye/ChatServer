package com.muduo.chat;
import com.google.protobuf.Message;


public class ChatMessage {
	private String type;
	private Message message;
	
	public ChatMessage(String type,Message message){
		this.type = type;
		this.message = message;
	}
	public String getType(){
		return type;
	}
	public Message getMessage(){
		return message;
	}
}
