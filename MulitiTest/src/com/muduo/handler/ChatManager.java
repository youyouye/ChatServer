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
	public int getClientId(){
		return client.getClientId();
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
	private Map<Link,Mess> sendMap;
	private Map<Link,Mess> recvMap;
	
	public synchronized void reSendAll(){
		logger.info("ChatManager:重发所有信息:"+ client.getClientId());
		for (Mess mess : sendMap.values()) {
			 client.rSendMess(mess.chatMess.getToid(),mess.chatMess.getId(),mess.chatMess.getMessage());
		}
	}
	public void receiveAck(int fromid,int toid,int rank){
		//无法想象这里的逻辑混乱了.ack代表的信息错误了正好相反.
		Link link = new Link(toid, fromid, rank);
		if (sendMap.containsKey(link)){
			sendMap.remove(link);
			//然后重启定时器.
			if (sendMap.isEmpty()){
				client.finishTimer(1);
			}else{
				client.resetTimer(1);
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
			client.sendAck(fromid, rank);
			return false;
		}
		Mess mess = new Mess(cm, 1);
		recvMap.put(link, mess);
		client.sendAck(fromid, rank);
		return true;
	}
	
	public void receiveOffRly(int page){
		if (page != -1){
			client.resetTimer(2);
		}else{
			//也就是说消息全部得到了，不需要再发送了.
			client.finishTimer(2);
			return;
		}
		client.pullOfflineMsg(page);
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
