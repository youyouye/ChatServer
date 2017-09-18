package com.muduo.testcase;

import java.util.HashMap;
import java.util.Map;


public class TestMap {
	public static class Mess{
		public int status;//发送成功,发送未ack
		public Mess(int status){
			this.status = status;
		}
	}
	public static class Link{
		public int fromid;
		public int toid;
		public int rank;
		public Link(int fromid,int toid,int rank){
			this.fromid = fromid;
			this.toid = toid;
			this.rank = rank;
		}
	}
	public Map<Link, Mess> map = new HashMap<>(); 
	public static void main(String[] args){
		Link link = new Link(123, 456, 1);
		Mess mess = new Mess(1);
		TestMap tm = new TestMap();
		tm.map.put(link, mess);
		
		boolean t = tm.map.containsKey(link);
		if (tm.map.get(link) == null){
			System.out.println("??");
		}
		
		System.out.println(t);
	}
}
