package com.muduo.testcase;

import com.muduo.chat.ChatClient;
import com.muduo.chat.TestCase;

public class TestOfflineMsg extends TestCase{

	@Override
	public void run() {
		final ChatClient client1 = god.newClient(123);
		client1.sendConnect();
		for (int i = 0;i < 100;i++){
			client1.sendChatMessage(456, "hello++"+i);
		}
		
	//	final ChatClient client2 = god.newClient(456);
	//	client2.sendConnect();
	//	client2.pullOfflineMsg(0);
	}
}
