package com.muduo.testcase;

import com.muduo.chat.ChatClient;
import com.muduo.chat.TestCase;

public class TestTwoClientChat extends TestCase{

	@Override
	public void run() {
		if (!queue.isEmpty()){
            fail("EventQueue is not empty");
		}
		
		// First Client:
		final ChatClient client1 = god.newClient(123);
		final ChatClient client2 = god.newClient(456);
		
		
		client1.sendConnect();
		client2.sendConnect();
		
		client1.sendChatMessage(456, "Hello!How are you?");
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		client2.sendChatMessage(123, "I'm fine,and you?");
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		client1.sendChatMessage(456, "I'm ok.");
	}
	
}
