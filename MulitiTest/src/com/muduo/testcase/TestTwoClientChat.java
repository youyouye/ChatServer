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
		client1.sendChatMessage(123, 456);
		
		
	}
	
}
