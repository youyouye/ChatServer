/*
 * GroupServer.h
 *
 *  Created on: Apr 9, 2017
 *      Author: moocos
 */

#ifndef GROUPSERVER_H_
#define GROUPSERVER_H_

#include "codec.h"
#include "group.pb.h"
#include <boost/noncopyable.hpp>

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Timestamp.h>

#include <vector>
#include <map>

using namespace muduo;
using namespace muduo::net;

typedef boost::shared_ptr<group::HandleGroup> HandleGroupPtr;
typedef boost::shared_ptr<group::GroupMessage> GroupMessagePtr;

class ChatServer;

class Group
{
public:
	Group(std::string name,Timestamp now)
	: name_(name),id_(0),create_time(now)
	{
	}
	void addMemeber(int id){
		members_.push_back(id);
	}
	void deleteMember(int id){
	}
	void setCreateId(const int id){
		id_ = id;
	}
private:
	std::string name_;
	int id_; //创建者ID
	Timestamp create_time;
public:
	std::vector<int> members_;
};

class GroupServer : boost::noncopyable
{
public:
	GroupServer(ChatServer* server,ProtobufCodec& code)
	: groups_(new GroupMap()),chatserver_(server),codec_(code)
	{
	}
public:
	void onHandleGroup(const muduo::net::TcpConnectionPtr& conn,
            const HandleGroupPtr& message,
            muduo::Timestamp);
	void onGroupMessage(const muduo::net::TcpConnectionPtr& conn,
            const GroupMessagePtr& message,
            muduo::Timestamp);
	string inspector();
private:
	Group* getGroup(const std::string name);
	void publish(const std::string name,int id,const std::string content);
	typedef std::map<std::string,boost::shared_ptr< Group > > GroupMap;
	typedef boost::shared_ptr<GroupMap> GroupMapPtr;
	GroupMapPtr getGroupsPtr()
	{
		MutexLockGuard lock(mutex_group);
		return groups_;
	}

	MutexLock mutex_group;
	GroupMapPtr groups_;
	ChatServer* chatserver_;
	ProtobufCodec& codec_;
};




#endif /* GROUPSERVER_H_ */
