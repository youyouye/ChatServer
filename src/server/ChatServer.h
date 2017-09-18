/*
 * ChatServer.h
 *
 *  Created on: Apr 2, 2017
 *      Author: moocos
 */
#ifndef CHATSERVER_H_
#define CHATSERVER_H_


#include "chat.pb.h"
#include "codec.h"
#include "dispatcher.h"
#include "GroupServer.h"

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/base/ThreadPool.h>
#include <boost/bind.hpp>

#include <map>
#include <hash_map>
#include <list>

using namespace muduo;
using namespace muduo::net;
using namespace __gnu_cxx;

typedef boost::shared_ptr<chat::Connect> ConnectPtr;
typedef boost::shared_ptr<chat::ChatMessage> ChatMessagePtr;
typedef boost::shared_ptr<chat::heart> HeartPtr;
typedef boost::shared_ptr<chat::ChatAck> ChatAckPtr;
typedef boost::shared_ptr<chat::OffMsgAsk> OffMsgAskPtr;
typedef boost::shared_ptr<chat::FriendAck> FriendAckPtr;
typedef boost::shared_ptr<chat::FriendAsk> FriendAskPtr;
typedef boost::shared_ptr<chat::FriendRly> FriendRlyPtr;

typedef boost::weak_ptr<TcpConnection> WeakTcpConnectionPtr;


//FIXME:需要用enable_from_this吗?能直接传this吗?
class ChatServer : boost::noncopyable
{
public:
	ChatServer(EventLoop* loop,
            const InetAddress& listenAddr);
	void start();
	void setThreadNum(int numThreads)
	{
	    server_.setThreadNum(numThreads);
	}
	TcpConnectionPtr getConnById(int id);
	GroupServer& getGroupServer(){
		return groupserver_;
	}
	string inspector();
private:

	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr&,
            const string& message,
            Timestamp);


	void onUnknownMessage(const TcpConnectionPtr& conn,
	                        const MessagePtr& message,
	                        Timestamp);

	void onConnect(const muduo::net::TcpConnectionPtr& conn,
            const ConnectPtr& message,
            muduo::Timestamp);
	void onChat(const muduo::net::TcpConnectionPtr& conn,
            const ChatMessagePtr& message,
            muduo::Timestamp);
	void onDataChat(const muduo::net::TcpConnectionPtr& conn,int toid,int serialId,int fromid,std::string content);
	void onHeart(const muduo::net::TcpConnectionPtr& conn,
            const HeartPtr& message,
            muduo::Timestamp);
	void onChatAck(const muduo::net::TcpConnectionPtr& conn,
            const ChatAckPtr& message,
            muduo::Timestamp);
	void onOffSingleMsg(const muduo::net::TcpConnectionPtr& conn,
            const OffMsgAskPtr& message,
            muduo::Timestamp);
	void onFriendAsk(const muduo::net::TcpConnectionPtr& conn,
            const FriendAskPtr& message,
            muduo::Timestamp);
	void onFriendRly(const muduo::net::TcpConnectionPtr& conn,
            const FriendRlyPtr& message,
            muduo::Timestamp);
	void onFriendAck(const muduo::net::TcpConnectionPtr& conn,
            const FriendAckPtr& message,
            muduo::Timestamp);
	void onTimer(void);
	void dumpConnectionList() const;
	void printThroughput();
	struct Entry : public muduo::copyable
	{
	    explicit Entry(const WeakTcpConnectionPtr& weakConn)
	      : weakConn_(weakConn)
	    {
	    }

	    ~Entry()
	    {
	      muduo::net::TcpConnectionPtr conn = weakConn_.lock();
	      if (conn)
	      {
	        conn->shutdown();
	      }
	    }
	    WeakTcpConnectionPtr weakConn_;
	};

	struct Node : public muduo::copyable
	{
		Timestamp lastReceiveTime;
		int connId;
		Node(Timestamp now,int id)
		{
			lastReceiveTime = now;
			connId = id;
		}
	};

	typedef boost::shared_ptr<Entry> EntryPtr;
	typedef boost::weak_ptr<Entry> WeakEntryPtr;
	typedef hash_map<int,EntryPtr> ConnectionList;
	typedef std::list< boost::shared_ptr< Node > > ListNode;
	typedef hash_map<int,int> IdMap;

	typedef boost::shared_ptr<ConnectionList> ConnectionListPtr;
	typedef boost::shared_ptr<ListNode> ListNodesPtr;
	typedef boost::shared_ptr<IdMap> IdMapPtr;

	ConnectionListPtr getConnectionList()
	{
		MutexLockGuard lock(mutex_conn);
		return connections_;
	}
	ListNodesPtr getListNode()
	{
		MutexLockGuard lock(mutex_conn);
		return nodes;
	}
	IdMapPtr getIdMap()
	{
		MutexLockGuard lock(mutex_id);
		return idVuid;
	}

	TcpServer server_;
	IdMapPtr idVuid;
	ConnectionListPtr connections_;
	AtomicInt32 index;
	//用于性能测评
	static AtomicInt64 transNum;
	int64_t oldCountRecv_;
	int64_t oldCountSend_;
	double recvResult,sendResult,transResult;
	Timestamp startTime_;
	MutexLock mutex_conn;
	MutexLock mutex_id;
	ListNodesPtr nodes;

	ProtobufDispatcher dispatcher_;
	ProtobufCodec codec_;
	GroupServer groupserver_;
	class UserWithCid{
	public:
		UserWithCid(int ud,int cd){
			uid = ud;
			cid = cd;
		}
		int uid;
		int cid;
	};
	class list_finder{
		public:
			list_finder(int uid):id(uid){}
			bool operator()(const ListNode::value_type value)
			{
				return value->connId == id;
			}
		private:
			int id;
	};
	ThreadPool pool;
};

#endif /* CHATSERVER_H_ */
