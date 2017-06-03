package com.muduo.handler;

import java.util.HashMap;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.muduo.chat.ChatClient;
import com.muduo.proto.ChatProtos;

public class ChatManager {
	private static final Logger logger = LoggerFactory.getLogger("ChatManager");

	private ChatClient client;
	static int serialId = 1;
	public ChatManager(){
		sendMap = new HashMap<>();
		recvMap = new HashMap<>();
	}
	public void setClient(ChatClient cl){
		this.client = cl;
	}
	private class Mess{
		public ChatProtos.ChatMessage chatMess;
		public int status;//发送成功,发送未ack
		public Mess(ChatProtos.ChatMessage cm,int status){
			this.chatMess = cm;
			this.status = status;
		}
	}
	private class Link {
		public int fromid;
		public int toid;
		public int rank;
		public Link(int fromid,int toid,int rank){
			this.fromid = fromid;
			this.toid = toid;
			this.rank = rank;
		}
		public boolean equals(Object object){
			Link lk = (Link)object;
			if (this.fromid == lk.fromid && this.toid == lk.toid && this.rank == lk.rank){
				return true;
			}else{
				return false;
			}
		}
		public int hashCode(){
			return this.fromid + this.toid + this.rank;
		}
	}
	private static Map<Link,Mess> sendMap;
	private static Map<Link,Mess> recvMap;
	
	public void reSendAll(){
		logger.info("ChatManager:重发所有信息");
		for (Mess mess : sendMap.values()) {
			 client.rSendMess(mess.chatMess);
		}
	}
	public void receiveAck(int fromid,int toid,int rank){
		Link link = new Link(fromid, toid, rank);
		if (sendMap.containsKey(link)){
			sendMap.remove(link);
			//然后重启定时器.
			if (sendMap.isEmpty()){
				client.finishTimer();
			}else{
				client.resetTimer();
			}
		}
	}
	//如果不重复就true
	public boolean receiveMess(ChatProtos.ChatMessage cm){
		int fromid = cm.getFromid();
		int toid = cm.getToid();
		int rank = cm.getId();
		Link link = new Link(fromid, toid, rank);
		if (recvMap.containsKey(link)){
			client.sendAck(fromid, toid, rank);
			return false;
		}
		Mess mess = new Mess(cm, 1);
		recvMap.put(link, mess);
		client.sendAck(fromid, toid, rank);
		return true;
	}
	public void sendMess(ChatProtos.ChatMessage chatMess){
		int fromid = chatMess.getFromid();
		int toid = chatMess.getToid();
		int rank = chatMess.getId();
		Link link = new Link(fromid, toid, rank);
		Mess mess = new Mess(chatMess,1);
		sendMap.put(link, mess);
	}
	public int getSerialId(){
		return serialId++;
	}
}
