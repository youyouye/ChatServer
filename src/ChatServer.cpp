/*
 * ChatServer.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: moocos
 */

#include "ChatServer.h"
#include "database/online_user.h"
#include "database/mysql_conn_pool.h"
#include <stdio.h>
#include "database/offline_msg.h"
typedef boost::shared_ptr<std::vector<SingleOfflineMsg> > SingleOffMsgListPtr;

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
	dispatcher_.registerMessageCallback<chat::ChatAck>(boost::bind(&ChatServer::onChatAck, this, _1, _2, _3));
	dispatcher_.registerMessageCallback<group::HandleGroup>(boost::bind(&GroupServer::onHandleGroup, &groupserver_, _1, _2, _3));
	dispatcher_.registerMessageCallback<group::GroupMessage>(boost::bind(&GroupServer::onGroupMessage, &groupserver_, _1, _2, _3));
	dispatcher_.registerMessageCallback<chat::heart>(boost::bind(&ChatServer::onHeart, this, _1, _2, _3));
	dispatcher_.registerMessageCallback<chat::OffMsgAsk>(boost::bind(&ChatServer::onOffSingleMsg,this,_1,_2,_3));
	dispatcher_.registerMessageCallback<chat::FriendAck>(boost::bind(&ChatServer::onFriendAck,this,_1,_2,_3));
	dispatcher_.registerMessageCallback<chat::FriendAsk>(boost::bind(&ChatServer::onFriendAsk,this,_1,_2,_3));
	dispatcher_.registerMessageCallback<chat::FriendRly>(boost::bind(&ChatServer::onFriendRly,this,_1,_2,_3));

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
    	conn->setContext(UserWithCid(0,tindex));
    	EntryPtr entry(new Entry(conn));
    	connections_->insert({tindex,entry});
    	boost::shared_ptr<Node> node(new Node(Timestamp::now(),tindex));
    	nodes->push_back(node);
    }else{
    	UserWithCid uwc = boost::any_cast<UserWithCid>(conn->getContext());
    	int id = uwc.cid;
    	ConnectionList::iterator iter = connections_->find(id);
    	connections_->erase(iter);
    	ListNode::iterator listiter = std::find_if(nodes->begin(),nodes->end(),list_finder(id));
    	if (listiter != nodes->end()){
    		nodes->erase(listiter);
    	}
    	LOG_INFO<<"修改在线状态:"<<uwc.uid<<":下线";
    	OnlineUser::instance()->alterUser(uwc.uid,0);
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
	UserWithCid uwc = boost::any_cast<UserWithCid>(conn->getContext());
	int uid = uwc.cid;
	(*idVuid)[id] = uid;
	conn->setContext(UserWithCid(id,uid));
	//添加在线状态
	LOG_INFO<<"修改在线状态:"<<id<<":上线";
	OnlineUser::instance()->alterUser(id,1);
}
void ChatServer::onChat(const muduo::net::TcpConnectionPtr& conn,
        const ChatMessagePtr& message,
        muduo::Timestamp)
{
	LOG_INFO<<"onChat"<<message->toid()<<"&"<<message->message();
	int fromid = message->fromid();
	int toid = message->toid();
	int time = message->time();
	int serialId = message->id();
  	std::string content = message->message();
	chat::ChatMessage chat;
	chat.set_fromid(fromid);
	chat.set_toid(toid);
	chat.set_time(time);
	chat.set_message(content);
	chat.set_id(serialId);
	ConnectionListPtr connections = getConnectionList();
    IdMapPtr idMapPtr = getIdMap();

	//添加离线信息管理.
	int status = OnlineUser::getStatus(toid);
	if (status == 0){
		LOG_INFO<<"对方不在线";
		SingleOfflineMsg sm(toid,serialId,fromid,1,content);
		int res = MysqlConnPool::instance()->getNextConn()->addSingleOfflineMsg(sm);
		LOG_INFO<<"离线消息插入数据库:返回"<<res;
		if (res >0){
			chat::ChatAck ack;
			ack.set_fromid(toid);
			ack.set_toid(fromid);
			ack.set_time(20170614);
			ack.set_id(serialId);
			codec_.send(conn,ack);
			LOG_INFO<<"发送离线ack";
			LOG_INFO<<"离线ack内容"<<fromid<<":"<<toid<<":"<<time<<":"<<serialId;
		}
		return;
	}
	int index = -1;
	IdMap::iterator idmap = idMapPtr->find(toid);
	if (idmap != idMapPtr->end()){
		index = idmap->second;
	}
	ConnectionList::iterator it = connections->find(index);
	if ( it!= connections->end()){
		TcpConnectionPtr ptr = it->second->weakConn_.lock();
		if (ptr){
				LOG_INFO<<"检测找到对应的id";
				codec_.send(ptr,chat);
		}
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

	UserWithCid uwc = boost::any_cast<UserWithCid>(conn->getContext());
	int id = uwc.cid;
	ListNode::iterator listiter = std::find_if(nodesPtr->begin(),nodesPtr->end(),list_finder(id));
	(*listiter)->lastReceiveTime = time;
	nodesPtr->splice(nodesPtr->end(),*nodesPtr,listiter);

	dumpConnectionList();
}

void ChatServer::onChatAck(const muduo::net::TcpConnectionPtr& conn,
        const ChatAckPtr& message,
        muduo::Timestamp)
{
	LOG_INFO<<"onChatAck:"<<message->fromid()<<"&"<<message->toid();
	int fromid = message->fromid();
	int toid = message->toid();
	int time = message->time();
  	int messageId = message->id();
	chat::ChatAck ack;
	ack.set_fromid(fromid);
	ack.set_toid(toid);
	ack.set_time(time);
	ack.set_id(messageId);

	ConnectionListPtr connections = getConnectionList();
    IdMapPtr idMapPtr = getIdMap();

	int index = -1;
	IdMap::iterator idmap = idMapPtr->find(toid);
	if (idmap != idMapPtr->end()){
		index = idmap->second;
	}
	ConnectionList::iterator it = connections->find(index);
	if ( it!= connections->end()){
		TcpConnectionPtr ptr = it->second->weakConn_.lock();
		if (ptr){
				LOG_INFO<<"检测找到对应的id";
				codec_.send(ptr,ack);
		}
	}
}

void ChatServer::onOffSingleMsg(const muduo::net::TcpConnectionPtr& conn,
        const OffMsgAskPtr& message,
        muduo::Timestamp)
{
//	LOG_INFO<<"OnOffSingleMsg:"<<message->page()<<"&"<<message->uid();
	int page = (*message).page();
	int uid = (*message).uid();
	//也就是没有离线信息了.
	if(page == -1){
		return;
	}
	SingleOffMsgListPtr ptr(new std::vector<SingleOfflineMsg>());
	int res = MysqlConnPool::instance()->getNextConn()->getSingleOffLineMsg(page,uid,ptr);
	if (res < 0){
		LOG_ERROR<<"查询失败";
	}else{
		if (ptr->size() == 0){
			LOG_INFO<<"返回数量为0";
			chat::OffMsgRly rly;
			rly.set_page(-1);
			codec_.send(conn,rly);
		}else{
			LOG_INFO<<"返回数量>0";
			chat::OffMsgRly rly;
			rly.set_page(page+1);
			for (std::vector<SingleOfflineMsg>::iterator it = ptr->begin();it != ptr->end();it++){
				chat::ChatMessage* cm = rly.add_messages();
				cm->set_fromid((*it).send_uid);
				cm->set_toid((*it).receive_uid);
				cm->set_time(0); //error:暂时不填
				chat::ChatMessage::MsgType type = (*it).msg_type;
				cm->set_type(type);
				cm->set_id((*it).msg_id);
				cm->set_message((*it).msg_content);
			}
			codec_.send(conn,rly);
		}
	}
}

void ChatServer::onFriendAsk(const muduo::net::TcpConnectionPtr& conn,
        const FriendAskPtr& message,
        muduo::Timestamp)
{
	LOG_INFO << "onFriendAsk:"<<message->askid()<<"&"<<message->reqid();
	chat::FriendAsk ask;
	ask.set_askid(message->askid());
	ask.set_reqid(message->reqid());
	ask.set_askmessage(message->askmessage());

	ConnectionListPtr connections = getConnectionList();
    IdMapPtr idMapPtr = getIdMap();

	//添加离线信息管理.
	int status = OnlineUser::getStatus(message->reqid());
	if (status == 0){
		LOG_INFO<<"对方不在线";
		FriendOffMsg fm(message->askid(),message->reqid(),message->askmessage());
		int res = MysqlConnPool::instance()->getNextConn()->addOffFriendAsk(fm);
		LOG_INFO<<"离线好友请求插入数据库:返回"<<res;
		if (res >0){
			chat::FriendAck ack;
			ack.set_fromid(message->askid());
			ack.set_toid(message->reqid());
			codec_.send(conn,ack);
			LOG_INFO<<"发送添加好友离线ack";
		}
		return;
	}
	int index = -1;
	IdMap::iterator idmap = idMapPtr->find(message->reqid());
	if (idmap != idMapPtr->end()){
		index = idmap->second;
	}
	ConnectionList::iterator it = connections->find(index);
	if ( it!= connections->end()){
		TcpConnectionPtr ptr = it->second->weakConn_.lock();
		if (ptr){
				LOG_INFO<<"检测找到对应的id";
				codec_.send(ptr,ask);
		}
	}
}
void ChatServer::onFriendRly(const muduo::net::TcpConnectionPtr& conn,
        const FriendRlyPtr& message,
        muduo::Timestamp)
{
	LOG_INFO << "onFriendRly:"<<message->sendid()<<"&"<<message->recvid();
	chat::FriendRly rly;
	rly.set_recvid(message->recvid());
	rly.set_sendid(message->sendid());
	rly.set_reply(message->reply());

	ConnectionListPtr connections = getConnectionList();
    IdMapPtr idMapPtr = getIdMap();

	//添加离线信息管理.
	int status = OnlineUser::getStatus(message->recvid());
	if (status == 0){
		LOG_INFO<<"对方不在线";
		FriendRlyMsg fm(message->sendid(),message->recvid(),message->reply());
		int res = MysqlConnPool::instance()->getNextConn()->addOffFriendRly(fm);
		LOG_INFO<<"离线好友回应插入数据库:返回"<<res;
		if (res >0){
			chat::FriendAck ack;
			ack.set_fromid(message->sendid());
			ack.set_toid(message->recvid());
			codec_.send(conn,ack);
			LOG_INFO<<"发送添加好友回应ack";
		}
		return;
	}
	int index = -1;
	IdMap::iterator idmap = idMapPtr->find(message->recvid());
	if (idmap != idMapPtr->end()){
		index = idmap->second;
	}
	ConnectionList::iterator it = connections->find(index);
	if ( it!= connections->end()){
		TcpConnectionPtr ptr = it->second->weakConn_.lock();
		if (ptr){
				LOG_INFO<<"检测找到对应的id";
				codec_.send(ptr,rly);
		}
	}

}
void ChatServer::onFriendAck(const muduo::net::TcpConnectionPtr& conn,
        const FriendAckPtr& message,
        muduo::Timestamp)
{
	LOG_INFO << "onFriendAck:"<<message->fromid()<<"&"<<message->toid();
	chat::FriendAck ack;
	ack.set_fromid(message->fromid());
	ack.set_toid(message->toid());
	ConnectionListPtr connections = getConnectionList();
    IdMapPtr idMapPtr = getIdMap();

	int index = -1;
	IdMap::iterator idmap = idMapPtr->find(ack.toid());
	if (idmap != idMapPtr->end()){
		index = idmap->second;
	}
	ConnectionList::iterator it = connections->find(index);
	if ( it!= connections->end()){
		TcpConnectionPtr ptr = it->second->weakConn_.lock();
		if (ptr){
				LOG_INFO<<"检测找到对应的id";
				codec_.send(ptr,ack);
		}
	}

}

void ChatServer::onTimer(void)
{
//	dumpConnectionList();

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
	for (ConnectionList::iterator it = connections_->begin();it != connections_->end();it++){
		UserWithCid uwc = boost::any_cast<UserWithCid>(it->second->weakConn_.lock()->getContext());
		int id = uwc.cid;
		bool isE = it->second->weakConn_.lock()->connected();
		LOG_INFO<<"dumpConnection:"<<id<<"+"<<isE;
	}
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
