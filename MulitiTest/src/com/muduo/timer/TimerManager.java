package com.muduo.timer;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.muduo.handler.ChatManager;

public class TimerManager {
	private static final Logger logger = LoggerFactory.getLogger("TimerManager");
	
	private int faiure = 0;
    private ConcurrentHashMap<String, Future> futureMap = new ConcurrentHashMap<String, Future>();  
    private ConcurrentHashMap<String,MyTimer> timers = new ConcurrentHashMap<>();
    private ScheduledExecutorService service;
	private ChatManager chatManager;
	public TimerManager(ChatManager cm){
		service = Executors.newScheduledThreadPool(4);
		this.chatManager = cm;
	}
	
	public boolean isFirst(String tag){
		if (timers.get(tag) == null){
			timers.put(tag, new MyTimer());
		}
		return timers.get(tag).isFirst;
	}
	public boolean isEnd(String tag){
		return timers.get(tag).isEnd;
	}
	
	public void startTimer(String tag){
		logger.info("定时任务开启");
		MyTask task = new MyTask(tag, futureMap);
		Future future = service.scheduleAtFixedRate(task, 3, 3, TimeUnit.SECONDS);
		futureMap.put(tag, future);
		timers.get(tag).isFirst = false;
		timers.get(tag).isEnd = false;
		faiure = 0;
	}
	public void resetTimer(String tag){
		logger.info("定时任务重置");
		Future future = futureMap.get(tag);
		future.cancel(true);
		MyTask task = new MyTask(tag, futureMap);
		Future newFuture = service.scheduleAtFixedRate(task, 3, 3, TimeUnit.SECONDS);
		futureMap.put(tag, newFuture);
		faiure = 0;
	}
	public void endTimer(String tag){
		logger.info("定时任务停止");
		Future future = futureMap.get(tag);
		future.cancel(true);
		timers.get(tag).isEnd = true;
	}
	
	public class MyTask implements Runnable{
        private ConcurrentHashMap<String, Future> futureMap;
		private String tag;
        public MyTask(String tag,ConcurrentHashMap<String, Future> futureMap){
			this.futureMap = futureMap;
			this.tag = tag;
        }
		@Override
		public void run() {
			logger.info("定时任务执行");
			chatManager.reSendAll();
			//失败一次就开启一个长时间的定时器
			faiure ++;
			Future future = futureMap.get(tag);
			future.cancel(true);
			MyTask task = new MyTask(tag, futureMap);
			Future newFuture = service.scheduleAtFixedRate(task,faiure*4 , faiure*4, TimeUnit.SECONDS);
			futureMap.put(tag, newFuture);
		}
	};
}
