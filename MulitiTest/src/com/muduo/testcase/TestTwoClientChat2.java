package com.muduo.testcase;

import com.muduo.chat.ChatClient;
import com.muduo.chat.TestCase;

public class TestTwoClientChat2 extends TestCase{

	@Override
	public void run() {
		if (!queue.isEmpty()){
            fail("EventQueue is not empty");
		}
		
		// First Client:
		final ChatClient client1 = god.newClient(123);
		client1.sendConnect();
		client1.sendChatMessage(456, "hello!+1");
		client1.sendChatMessage(456, "hello!+2");
		client1.sendChatMessage(456, "hello!+3");
		client1.sendChatMessage(456, "hello!+4");
		client1.sendChatMessage(456, "hello!+5");

	}
	
}
