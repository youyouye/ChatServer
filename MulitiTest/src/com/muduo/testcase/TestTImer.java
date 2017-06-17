package com.muduo.testcase;

import java.util.Timer;
import java.util.TimerTask;

public class TestTImer extends TimerTask{
	Timer timer;
	boolean isEnd = false;
	int i = 0;
	public void start(){
		timer = new Timer();
		timer.schedule(this, 1000,1000);
	}
	@Override
	public void run() {
		// TODO Auto-generated method stub
		i++;
	}
	public void finish(){
		if (i == 3){
			timer.cancel();
		}
	}
	public void testThread(){
		new MultiThread().start();
	}
	class MultiThread extends Thread{
		public void run(){
			isEnd = true;
		}
	}
	public static void main(String[] args){
		TestTImer tt = new TestTImer();
		tt.start();
		System.out.println(tt.isEnd);
		tt.testThread();
		while(tt.i < 10){
			System.out.println(tt.i);
			System.out.println(tt.isEnd);
		}
	}
}
