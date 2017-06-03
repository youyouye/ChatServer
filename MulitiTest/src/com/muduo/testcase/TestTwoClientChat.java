package com.muduo.testcase;

import java.util.Scanner;

import com.muduo.chat.ChatClient;
import com.muduo.chat.TestCase;

public class TestTwoClientChat extends TestCase{

	@Override
	public void run() {
		if (!queue.isEmpty()){
            fail("EventQueue is not empty");
		}
		
		// First Client:
		final ChatClient client1 = god.newClient();
		final ChatClient client2 = god.newClient();
		client1.sendConnect(123);
		client2.sendConnect(456);
		
		client1.sendChatMessage(123,456, "Hello!How are you?");
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		client2.sendChatMessage(456, 123, "I'm fine,and you?");
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		client1.sendChatMessage(123, 456, "I'm ok.");
	}
	
}
