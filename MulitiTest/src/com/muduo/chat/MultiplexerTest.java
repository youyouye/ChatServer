package com.muduo.chat;

import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;

import java.net.InetSocketAddress;
import java.util.ArrayList;
import java.util.regex.Pattern;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.muduo.testcase.TestTwoClientChat;

public class MultiplexerTest {
	private static final Logger logger = LoggerFactory.getLogger("MultiplexerTest");
	
    public final Pattern commandChannel = Pattern.compile("CONN (\\d+) FROM [0-9.:]+ IS ([A-Z]+)\r\n");

    private static final int kMultiplexerServerPort = 2017;
    private static final int kLogicalServerPort = 9999;
    private final InetSocketAddress multiplexerAddress;
	private final EventLoopGroup workerGroup;
	
	private EventQueue queue;
	private MyCountDownLatch latch;
	private ArrayList<TestCase> testCases;
	
	public MultiplexerTest(String multiplexerHost){
		multiplexerAddress = new InetSocketAddress(multiplexerHost, kMultiplexerServerPort);
		workerGroup = new NioEventLoopGroup();
		queue = new EventQueue();
		latch = new MyCountDownLatch(1);
		testCases = new ArrayList<TestCase>();
	}
    
	private void addTestCase(TestCase testCase){
		testCases.add(testCase);
		testCase.setOwner(this);
	}
	
	public ChatClient newClient(){
		ChatClient client = new ChatClient(queue, multiplexerAddress, workerGroup);
		client.connectAndWait();
		return client;
	}
	
	public EventQueue getEventQueue(){
		return queue;
	}
	
	public void sleep(int millis){
		try{
			Thread.sleep(millis);
		}catch(InterruptedException e){
			e.printStackTrace();
		}
	}
	
	private void run(){
		logger.info("Waiting for connection");
		for (TestCase testCase : testCases){
			testCase.test();
		}
	}
	
	
	public static void main(String[] args){
		String multiplexerHost = "127.0.0.1";
		MultiplexerTest test = new MultiplexerTest(multiplexerHost);
		test.addTestCase(new TestTwoClientChat());
		test.run();
	}
}
