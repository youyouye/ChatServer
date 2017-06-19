package com.muduo.testcase;

import java.util.ArrayList;

import com.muduo.chat.ChatClient;
import com.muduo.chat.TestCase;

public class TestMultiClient extends TestCase{
	@Override
	public void run() {
		new Thread(new T1()).start();
	}
	public class T1 implements Runnable{
		ArrayList<ChatClient> clients = new ArrayList<>();
		public void run() {
			for (int i = 0;i < 50;i++){
				ChatClient client = god.newClient(i+1);
				client.sendConnect();
				clients.add(client);
			}
			try {
				Thread.sleep(2000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			int i = 0;
			while(i<49){
				clients.get(i).sendChatMessage(i+2, "Hello!!");
				i++;
//				while (i == 48){
//					i = 0;
//				}
			}
			for (int tt = 0;tt<clients.size();tt++){
				clients.get(tt).getChatManager().printTimes();
			}
			
		}
	}
}
