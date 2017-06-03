package com.muduo.chat;
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
import com.muduo.handler.ChatManager;
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
	//控制的全局Mess信息.
	private ChatManager chatManager;
	private MyTimer myTimer;
	
	public ChatClient(EventQueue queue, InetSocketAddress remoteAddress,EventLoopGroup workerGroup){
		this.queue = queue;
		this.remoteAddress = remoteAddress;
		this.workerGroup = workerGroup;
		this.timer = new HashedWheelTimer();
		chatManager = new ChatManager();
		chatManager.setClient(this);
		myTimer = new MyTimer(chatManager);
		connId = -1;
	}
	
	public void connect(){
		assert bootstrap == null;	
		
		try{
			bootstrap = new Bootstrap();
			bootstrap.group(workerGroup);
			bootstrap.channel(NioSocketChannel.class);
			bootstrap.option(ChannelOption.SO_KEEPALIVE, true);
			
			bootstrap.handler(new ProtobufIntializer(lite,chatManager));
				
			connection = bootstrap.connect(remoteAddress).sync().channel();
			
		} catch (InterruptedException e) {
			workerGroup.shutdownGracefully();
			e.printStackTrace();
		}finally{
		}
		
	}
	
	public void connectAndWait(){
		latch = new MyCountDownLatch(1);
		connect();
		latch.awaitUninterruptibly(500);
		assert connection != null;
	}
	
	public void sendConnect(int from){
		logger.info("要发connect");
		ChatProtos.Connect connect = ChatProtos.Connect.newBuilder()
				.setId(from)
				.build();
		ChatMessage cm = new ChatMessage("chat.Connect\0",connect);
		connection.writeAndFlush(cm);
	}
	
	public void rSendMess(com.muduo.proto.ChatProtos.ChatMessage cm){
		logger.info("重发信息");
		ChatMessage chatmessage = new ChatMessage("chat.ChatMessage\0",cm);
		connection.writeAndFlush(chatmessage);
	}
	public void sendAck(int fromid,int toid,int serialId){
		logger.info("要发ack;");
		ChatProtos.ChatAck cAck = ChatProtos.ChatAck.newBuilder()
				.setFromid(fromid)
				.setToid(toid)
				.setTime((int)(System.currentTimeMillis() / 1000L + 2208988800L))
				.setId(serialId).build();
		ChatMessage cm = new ChatMessage("chat.ChatAck\0",cAck);
		connection.writeAndFlush(cm);
	}
	public void sendChatMessage(int from,int to,String mess){
		logger.info("要发信息"+mess);
        ChannelFuture lastWriteFuture = null;
		try {
			final String input = mess;
			if (input == null || "quit".equalsIgnoreCase(input)){
				connection.close().sync();
			}else if (input.isEmpty()){
			}
			ChatProtos.ChatMessage message = ChatProtos.ChatMessage.newBuilder()
					.setFromid(from)
					.setToid(to)
					.setMessage(input)
					.setId(chatManager.getSerialId())
					.setTime((int)(System.currentTimeMillis() / 1000L + 2208988800L)).build();
				
			ChatMessage cm = new ChatMessage("chat.ChatMessage\0",message);
			chatManager.sendMess(message);
			/*
			 * 要做什么了?需要定时器.
			 * 判断定时器时候开启了;同时接受到ack.就涉及ack和消息的id问题了.发送超时的问题.重启定时器.
			 * 我们需要让消息具有序号,假定初始都是100;消息如何与序号对应了,需要让每一条消息具有序号?如果我要重发消息的话,岂不是有个消息保存的问题.
		     * 那么需要保存多少消息,5,6条.根据人数来说的话还真是会很多唉?
			 */
				if (myTimer.isFirst || myTimer.isEnd){
					logger.info("执行了几次?");
					myTimer.start();
				}
				lastWriteFuture = connection.writeAndFlush(cm);
                lastWriteFuture.addListener(new GenericFutureListener<ChannelFuture>() {
                    @Override
                    public void operationComplete(ChannelFuture future) throws Exception {
                        if (!future.isSuccess()) {
                            System.err.print("write failed: ");
                            future.cause().printStackTrace(System.err);
                        }
                    }
                });
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
	}
	
	public void disconnect(){
		connection.close();
	}
	
	public void setId(int connId){
		assert this.connId == -1;
		this.connId = connId;
	}
	
	public void resetTimer(){
		myTimer.reset();
	}
	public void finishTimer(){
		myTimer.end();
	}
}
