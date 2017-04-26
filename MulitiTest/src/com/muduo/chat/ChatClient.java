package com.muduo.chat;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetSocketAddress;

import io.netty.bootstrap.Bootstrap;
import io.netty.channel.Channel;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelOption;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.socket.nio.NioSocketChannel;
import io.netty.util.HashedWheelTimer;
import io.netty.util.Timer;
import io.netty.util.concurrent.GenericFutureListener;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.MessageLite;
import com.muduo.proto.ChatProtos;


public class ChatClient {
	private static final Logger logger = LoggerFactory.getLogger("ChatClient");
	
	private final EventLoopGroup workerGroup;
	private final InetSocketAddress remoteAddress;
	private final EventQueue queue;
	private final Timer timer;
	
	private volatile Channel connection;
	private Bootstrap bootstrap;
	private int connId;
	private MyCountDownLatch latch;
	private MessageLite lite;
	
	
	public ChatClient(EventQueue queue, InetSocketAddress remoteAddress,EventLoopGroup workerGroup){
		this.queue = queue;
		this.remoteAddress = remoteAddress;
		this.workerGroup = workerGroup;
		this.timer = new HashedWheelTimer();
		connId = -1;
	}
	
	public void connect(){
		assert bootstrap == null;	
		
		try{
			bootstrap = new Bootstrap();
			bootstrap.group(workerGroup);
			bootstrap.channel(NioSocketChannel.class);
			bootstrap.option(ChannelOption.SO_KEEPALIVE, true);
			
			bootstrap.handler(new ProtobufIntializer(lite));
				
			connection = bootstrap.connect(remoteAddress).sync().channel();
			
		} catch (InterruptedException e) {
			e.printStackTrace();
		}finally{
			workerGroup.shutdownGracefully();
		}
		
	}
	
	public void connectAndWait(){
		latch = new MyCountDownLatch(1);
		connect();
		latch.awaitUninterruptibly(500);
		assert connection != null;
	}
	
	public void sendChatMessage(int from,int to){
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
        ChannelFuture lastWriteFuture = null;
		for (;;){
			try {
				final String input = in.readLine();
				if (input == null || "quit".equalsIgnoreCase(input)){
					connection.close().sync();
					break;
				}else if (input.isEmpty()){
					continue;
				}
				ChatProtos.ChatMessage message = ChatProtos.ChatMessage.newBuilder()
						.setFromid(from)
						.setToid(to)
						.setMessage(input)
						.setTime((int)(System.currentTimeMillis() / 1000L + 2208988800L)).build();
				lastWriteFuture = connection.writeAndFlush(message);
                lastWriteFuture.addListener(new GenericFutureListener<ChannelFuture>() {
                    @Override
                    public void operationComplete(ChannelFuture future) throws Exception {
                        if (!future.isSuccess()) {
                            System.err.print("write failed: ");
                            future.cause().printStackTrace(System.err);
                        }
                    }
                });
			} catch (IOException e) {
				e.printStackTrace();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			
		}
	}
	
	public void disconnect(){
		connection.close();
	}
	
	public void setId(int connId){
		assert this.connId == -1;
		this.connId = connId;
	}
}
