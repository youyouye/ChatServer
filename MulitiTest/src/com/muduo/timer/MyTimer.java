package com.muduo.timer;

public class MyTimer{
	/*
	 * 我需要什么?定时器.设置定时时间.以及判断定时器时候超时!重启定时器!
	 * 定时器超时事件;细细一想的话,是针对单独的一个连接建立的定时器..
	 * 在一想的话,所有的发送都是发往服务端.所有的接收都是从服务端来的.
	 * 这样就简单多了..不用一大堆定时器了.
	 * 很有趣的一点,TCP是如何给每一个连接定时器的.这一点让我很晕.我对线程基础的原理有点
	 * 不明白..先做这个吧..
	 * */
	public boolean isRun = false;
	public boolean isFirst = true;
	public boolean isEnd = false;
	public String tag;
	public int failure = 0;
}
