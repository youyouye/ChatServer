package com.muduo.chat;
import java.net.InetSocketAddress;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;

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
import com.muduo.timer.MyTimer;
import com.muduo.timer.TimerManager;


public class ChatClient {
	private static final Logger logger = LoggerFactory.getLogger("ChatClient");
	
	private final EventLoopGroup workerGroup;
	private final InetSocketAddress remoteAddress;
	private final EventQueue queue;
	
	private volatile Channel connection;
	private Bootstrap bootstrap;
	private int connId;
	private MyCountDownLatch latch;
	private MessageLite lite;
	//控制的全局Mess信息.
	private ChatManager chatManager;
	private TimerManager timeManager;
	//客户的id；
	private int clientID;
	//定时器
	public ScheduledExecutorService serviceEx = Executors.newScheduledThreadPool(2);	
	
	public ChatClient(EventQueue queue, InetSocketAddress remoteAddress,EventLoopGroup workerGroup){
		this.queue = queue;
		this.remoteAddress = remoteAddress;
		this.workerGroup = workerGroup;
		chatManager = new ChatManager();
		chatManager.setClient(this);
		timeManager = new TimerManager(chatManager);
		connId = -1;
	}
	public void setClientId(int id){
		this.clientID = id;
	}
	public int getClientId(){
		return clientID;
	}
	public ChatManager getChatManager(){
		return chatManager;
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
	
	public EventLoopGroup getEventLoop(){
		return workerGroup;
	}
	
	public void connectAndWait(){
		latch = new MyCountDownLatch(1);
		connect();
		latch.awaitUninterruptibly(500);
		assert connection != null;
	}
	
	public void sendConnect(){
		logger.info("要发connect");
		ChatProtos.Connect connect = ChatProtos.Connect.newBuilder()
				.setId(clientID)
				.build();
		ChatMessage cm = new ChatMessage("chat.Connect\0",connect);
		connection.writeAndFlush(cm);
	}
	
	public void rSendMess(final int toid,final int serialId,final String content){
		logger.info("重发信息"+content+":"+clientID);
		ChatProtos.ChatMessage message = ChatProtos.ChatMessage.newBuilder()
				.setFromid(clientID)
				.setToid(toid)
				.setMessage(content)
				.setId(serialId)
				.setTime((int)(System.currentTimeMillis() / 1000L + 2208988800L)).build();
		ChatMessage chatmessage = new ChatMessage("chat.ChatMessage\0",message);		
		connection.writeAndFlush(chatmessage);
	}
	public void sendAck(int toid,int serialId){
		logger.info("要发ack;");
		ChatProtos.ChatAck cAck = ChatProtos.ChatAck.newBuilder()
				.setFromid(clientID)
				.setToid(toid)
				.setTime((int)(System.currentTimeMillis() / 1000L + 2208988800L))
				.setId(serialId).build();
		ChatMessage cm = new ChatMessage("chat.ChatAck\0",cAck);
		connection.writeAndFlush(cm);
	}
	public void sendChatMessage(int to,String mess){
	//	logger.info("要发信息"+mess);
        ChannelFuture lastWriteFuture = null;
		try {
			final String input = mess;
			if (input == null || "quit".equalsIgnoreCase(input)){
				connection.close().sync();
			}else if (input.isEmpty()){
			}
			ChatProtos.ChatMessage message = ChatProtos.ChatMessage.newBuilder()
					.setFromid(clientID)
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
				if (timeManager.isFirst("send") || timeManager.isEnd("send")){
					logger.info("执行了几次?");
					timeManager.startTimer("send");
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
	//拉取离线消息.
	public void pullOfflineMsg(int page){
		logger.info("拉取离线消息:"+page);
		ChatProtos.OffMsgAsk message = ChatProtos.OffMsgAsk.newBuilder()
					.setPage(page)
					.setUid(clientID).build();
		ChatMessage cm = new ChatMessage("chat.OffMsgAsk\0",message);
		connection.writeAndFlush(cm);
		if (timeManager.isFirst("off") || timeManager.isEnd("off")){
			logger.info("执行了几次?");
			timeManager.startTimer("off");
		}
	}
	//发送好友请求:
	public void sendFriendAsk(int uid,String content){
		ChatProtos.FriendAsk ask = ChatProtos.FriendAsk.newBuilder()
				.setAskid(clientID)
				.setReqid(uid)
				.setAskmessage(content).build();
		ChatMessage cm = new ChatMessage("chat.FriendAsk\0",ask);
		connection.writeAndFlush(cm);		
		//暂时不处理发送失败的情况了.这不是我想写的重点东西.
	}
	public void sendFriendRly(int uid,int reply){
		ChatProtos.FriendRly rly = ChatProtos.FriendRly.newBuilder()
				.setSendid(clientID)
				.setRecvid(uid)
				.setReply(reply).build();
		ChatMessage cm = new ChatMessage("chat.FriendRly\0",rly);
		connection.writeAndFlush(cm);		
		//暂时不处理发送失败的情况了.这不是我想写的重点东西.
	}
	public void sendFriendAck(int uid){
		ChatProtos.FriendAck ack = ChatProtos.FriendAck.newBuilder()
				.setFromid(clientID)
				.setToid(uid).build();
		ChatMessage cm = new ChatMessage("chat.FriendAck\0",ack);
		connection.writeAndFlush(cm);
	}
	
	public void disconnect(){
		connection.close();
	}
	
	public void setId(int connId){
		assert this.connId == -1;
		this.connId = connId;
	}
	
	public void resetTimer(int which){
		if (which == 1){
			timeManager.resetTimer("send");
		}else if (which == 2){
			timeManager.resetTimer("off");
		}
	}
	public void finishTimer(int which){
		if (which == 1){
			timeManager.endTimer("send");
		}else if (which == 2){
			timeManager.endTimer("off");
		}
	}
}
