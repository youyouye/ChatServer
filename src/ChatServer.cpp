/*
 * ChatServer.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: moocos
 */

#include "ChatServer.h"
#include <stdio.h>
ChatServer::ChatServer(EventLoop* loop,const InetAddress& listenAddr)
	:server_(loop,listenAddr,"ChatServer",TcpServer::kReusePort),
	 idVuid(new IdMap()),
	 connections_(new ConnectionList()),
	 nodes(new ListNode()),
	 dispatcher_(boost::bind(&ChatServer::onUnknownMessage, this, _1, _2, _3)),
	 codec_(boost::bind(&ProtobufDispatcher::onProtobufMessage, &dispatcher_, _1, _2, _3)),
	 groupserver_(this,codec_)
{
	dispatcher_.registerMessageCallback<chat::Connect>(boost::bind(&ChatServer::onConnect, this, _1, _2, _3));
	dispatcher_.registerMessageCallback<chat::ChatMessage>(boost::bind(&ChatServer::onChat, this, _1, _2, _3));
	dispatcher_.registerMessageCallback<group::HandleGroup>(boost::bind(&GroupServer::onHandleGroup, &groupserver_, _1, _2, _3));
	dispatcher_.registerMessageCallback<group::GroupMessage>(boost::bind(&GroupServer::onGroupMessage, &groupserver_, _1, _2, _3));
	dispatcher_.registerMessageCallback<chat::heart>(boost::bind(&ChatServer::onHeart, this, _1, _2, _3));
	server_.setConnectionCallback(boost::bind(&ChatServer::onConnection, this, _1));
	server_.setMessageCallback(boost::bind(&ProtobufCodec::onMessage, &codec_, _1, _2, _3));

	loop->runEvery(1,boost::bind(&ChatServer::onTimer,this));
}


void ChatServer::start()
{
    server_.start();
}

void ChatServer::onConnection(const TcpConnectionPtr& conn){
    LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");
    MutexLockGuard lock(mutex_conn);
    if (!connections_.unique()){
    	connections_.reset(new ConnectionList(*connections_));
    }
    if (!nodes.unique()){
    	nodes.reset(new ListNode(*nodes));
    }
    if (conn->connected()){
    	int tindex = index.getAndAdd(1);
    	conn->setContext(tindex);
    	EntryPtr entry(new Entry(conn));
    	connections_->insert({tindex,entry});
    	boost::shared_ptr<Node> node(new Node(Timestamp::now(),tindex));
    	nodes->push_back(node);
    }else{
    	int id = boost::any_cast<int>(conn->getContext());
    	ConnectionList::iterator iter = connections_->find(id);
    	connections_->erase(iter);
    	ListNode::iterator listiter = std::find_if(nodes->begin(),nodes->end(),list_finder(id));
    	if (listiter != nodes->end()){
    		nodes->erase(listiter);
    	}
    }
    dumpConnectionList();
}
void ChatServer::onMessage(const TcpConnectionPtr&,
        const string& message,
        Timestamp){

}

void ChatServer::onUnknownMessage(const TcpConnectionPtr& conn,
                        const MessagePtr& message,
                        Timestamp)
{
	LOG_INFO << "onUnknownMessage: " << message->GetTypeName();
	    conn->shutdown();
}

void ChatServer::onConnect(const muduo::net::TcpConnectionPtr& conn,
        const ConnectPtr& message,
        muduo::Timestamp)
{
	LOG_INFO << "onConnect"<<message->id();

	MutexLockGuard lock(mutex_id);
	if (!idVuid.unique()){
		idVuid.reset(new IdMap(*idVuid));
	}

	int id = message->id();
	//加一个id检测合法的问题.
	int uid = boost::any_cast<int>(conn->getContext());
	idVuid->insert({id,uid});
}
void ChatServer::onChat(const muduo::net::TcpConnectionPtr& conn,
        const ChatMessagePtr& message,
        muduo::Timestamp)
{
	LOG_INFO<<"onChat"<<message->toid()<<"&"<<message->message();
	int fromid = message->fromid();
	int toid = message->toid();
	int time = message->time();
  	std::string content = message->message();
	chat::ChatMessage chat;
	chat.set_fromid(fromid);
	chat.set_toid(toid);
	chat.set_time(time);
	chat.set_message(content);

	ConnectionListPtr connections = getConnectionList();
    IdMapPtr idMapPtr = getIdMap();

	int index = idMapPtr->find(toid)->second;
	TcpConnectionPtr ptr = connections->find(index)->second->weakConn_.lock();
	if (ptr){
		LOG_INFO<<"检测找到对应的id";
		codec_.send(ptr,chat);
	}
}
void ChatServer::onHeart(const muduo::net::TcpConnectionPtr& conn,
        const HeartPtr& message,
        muduo::Timestamp time)
{
	LOG_INFO<<"onHeart"<<message->GetTypeName();
	chat::heart heart;
	codec_.send(conn,heart);

	ListNodesPtr nodesPtr = getListNode();

	int id = boost::any_cast<int>(conn->getContext());
	ListNode::iterator listiter = std::find_if(nodesPtr->begin(),nodesPtr->end(),list_finder(id));
	(*listiter)->lastReceiveTime = time;
	nodesPtr->splice(nodesPtr->end(),*nodesPtr,listiter);

	dumpConnectionList();
}

void ChatServer::onTimer(void)
{
	dumpConnectionList();

	ListNodesPtr nodesPtr = getListNode();
	ConnectionListPtr connections = getConnectionList();
	for (ListNode::iterator it = nodesPtr->begin();it != nodesPtr->end();){
		int id = (*it)->connId;
		ConnectionList::iterator iter = connections->find(id);
		if (iter != connections->end()){
			TcpConnectionPtr conn =(*iter).second->weakConn_.lock();
			if (conn){
				Timestamp now = Timestamp::now();
				double age = timeDifference(now,(*it)->lastReceiveTime);
				if (age > 300){
					if (conn->connected()){
				          conn->shutdown();
				          LOG_INFO << "shutting down " << conn->name();
				          conn->forceCloseWithDelay(3.5);
					}
				}else if (age < 0){
			        LOG_WARN << "Time jump";
				}else{
					break;
				}
				++it;
			}else{
			    LOG_WARN << "Expired";
			}
		}
	}
}
void ChatServer::dumpConnectionList() const
{
//	LOG_INFO << "size = " << connections_.size();
}

TcpConnectionPtr ChatServer::getConnById(int id)
{
	IdMap::iterator iter = idVuid->find(id);
	if (iter != idVuid->end()){
		ConnectionList::iterator it = connections_->find((*iter).second);
		if (it != connections_->end()){
			return (*it).second->weakConn_.lock();
		}
	}
}
string ChatServer::inspector()
{
	string result;
	result += "ConnectionList:\n";
	result += "size = ";
	char buf[10];
	snprintf(buf,sizeof buf,"%d",connections_->size());
	result += buf;
	result += "\n";
	for (ConnectionList::iterator it = connections_->begin();it != connections_->end();it++){
		char buf1[10];
		snprintf(buf1,sizeof buf1,"%d",(*it).first);
		result += buf1;
		result += "->";
		if ((*it).second->weakConn_.lock()){
			result += "exist";
		}else{
			result += "not exist";
		}
		result +="\n";
	}
	result += "IdMap : \n";
	result += "size = ";
	result += idVuid->size();
	for (IdMap::iterator it = idVuid->begin();it != idVuid->end();it++){
		char buf2[10];
		snprintf(buf2,sizeof buf2,"%d",(*it).first);
		result += buf2;
		result += "->";
		char buf3[10];
		snprintf(buf3,sizeof buf3,"%d",(*it).second);
		result += buf3;
		result += "\n";
	}
	return result;
}
