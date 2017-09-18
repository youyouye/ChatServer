package com.muduo.timer;

import java.util.Timer;
import java.util.TimerTask;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.muduo.handler.ChatManager;

public class MyTimer{
	private static final Logger logger = LoggerFactory.getLogger("MyTImer");
	/*
	 * 我需要什么?定时器.设置定时时间.以及判断定时器时候超时!重启定时器!
	 * 定时器超时事件;细细一想的话,是针对单独的一个连接建立的定时器..
	 * 在一想的话,所有的发送都是发往服务端.所有的接收都是从服务端来的.
	 * 这样就简单多了..不用一大堆定时器了.
	 * 很有趣的一点,TCP是如何给每一个连接定时器的.这一点让我很晕.我对线程基础的原理有点
	 * 不明白..先做这个吧..
	 * */
	public static boolean isRun = false;
	public static boolean isFirst = true;
	public static boolean isEnd = false;

	public Timer timer ;
	public ChatManager cm;
	private int taskType; //同一个种定时器，离线获取和消息传送的.
	private MyTask task;
	public int failure = 0;
	public MyTimer(ChatManager cm,int type){
		timer = new Timer();
		this.cm = cm;
		this.taskType = type;
	}
	public void start(){
		timer = new Timer();
		task = new MyTask();
		timer.schedule(task, 2000,2000);
		isFirst = false;
		isEnd = false;
	}

	public void reset(){
		task.cancel();
		task = new MyTask();
		timer.schedule(task, 2000,2000);
		isRun = false;
		failure = 0;
	}
	public void end(){
		task.cancel();
		isEnd = true;
	}
	public boolean checkStop(){
		return isRun;
	}
	public boolean checkFirst(){
		return isFirst;
	}
	public class MyTask extends TimerTask{

		@Override
		public void run() {
			if (isEnd){
				this.cancel();
				return;
			}
			logger.info("定时器任务执行");
			isRun = true;
			if (taskType == 1){
				cm.reSendAll();
				failure ++;
				this.cancel();
				task = new MyTask();
				timer.schedule(task, failure*6000,6000);
			}else if (taskType == 2){
			}
		}
		
	}
}
