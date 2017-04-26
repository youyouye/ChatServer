/*
 * GroupServer.cpp
 *
 *  Created on: Apr 9, 2017
 *      Author: moocos
 */
#include "GroupServer.h"
#include "ChatServer.h"
#include <sstream>

void GroupServer::onHandleGroup(const muduo::net::TcpConnectionPtr& conn,
           const HandleGroupPtr& message,
           muduo::Timestamp)
{
	LOG_INFO<<"onHandleGroup"<<":from"<<message->id()<<";group:"<<message->name()<<";type:"<<message->type();
	LOG_INFO<<"onHandleGroup::groups_"<<groups_->size();
	std::string group_name = message->name();

    MutexLockGuard lock(mutex_group);
    if (!groups_.unique()){
    	groups_.reset(new GroupMap(*groups_));
    }

	GroupMap::iterator iter = groups_->find(group_name);

	int id = message->id();

	if (message->type() == group::HandleGroup::CREATE)
		{
			LOG_INFO<<"CREATE:"<<id;
			LOG_INFO<<"groups:"<<groups_->size();
			if (iter != groups_->end()){
				LOG_INFO<<"CREATE:FIND?:"<<groups_->size();
				return;
			}else{
				Timestamp now = Timestamp::now();
				boost::shared_ptr<Group> group(new Group(group_name,now));
				group->addMemeber(id);
				group->setCreateId(id);
				groups_->insert({group_name,group});
				assert(groups_->size() > 0);
				LOG_INFO<<"groups:"<<groups_->size();
			}
		}
	else if (message->type() == group::HandleGroup::DELETE)
		{
			if (iter != groups_->end()){
				 groups_->erase(iter);
			}else{
				return;
			}
		}
	else if (message->type() == group::HandleGroup::JOIN)
		{
			LOG_INFO<<"JOIN:"<<id;
			LOG_INFO<<"groups:"<<groups_->size();
			if (iter != groups_->end()){
				 (*iter).second->addMemeber(id);
			}else{
				return;
			}
		}
	else if (message->type() == group::HandleGroup::QUIT)
		{
			if (iter != groups_->end()){
				 (*iter).second->deleteMember(id);
			}else{
				return;
			}
		}
}
void GroupServer::onGroupMessage(const muduo::net::TcpConnectionPtr& conn,
           const GroupMessagePtr& message,
           muduo::Timestamp)
{
	LOG_INFO<<"onGroupMessage"<<":from:"<<message->id()<<";to:"<<message->name();
	std::string group_name = message->name();
	int id = message->id();
	std::string content = message->content();
	//然后是分发.
	LOG_INFO<<"publish:" << getGroup(group_name)->members_.size();
	std::vector<int> mem = getGroup(group_name)->members_;
	for (std::vector<int>::iterator it = mem.begin();it != mem.end();it++){
		group::GroupMessage gm;
		gm.set_name(group_name); gm.set_id(id); gm.set_content(content);
		TcpConnectionPtr ptr = chatserver_->getConnById(*it);
		if (ptr != NULL){
			codec_.send(ptr,gm);
		}
	}

}

Group* GroupServer::getGroup(const std::string name)
{
	GroupMapPtr ptr = getGroupsPtr();
	LOG_INFO<<"getGroup"<<ptr->size();
	GroupMap::iterator iter = ptr->find(name);
	if (iter != ptr->end()){
		return iter->second.get();
	}else{
		return NULL;
	}
}

void GroupServer::publish(const std::string name,int id,const std::string content)
{
}
string GroupServer::inspector()
{
	string result;
	result += "GroupMap:\n";
	result += "size = ";
	result += groups_->size();
	result += "\n";
	for (GroupMap::iterator it = groups_->begin();it != groups_->end();it++){
		result += (*it).first.c_str();
		result += "->\n";
		result += "   member:\n";
		result += (*it).second->members_.size();
		result += "\n";
		for (std::vector<int>::iterator iter = (*it).second->members_.begin();
				iter != (*it).second->members_.end();iter++){
			result +=(*iter);
			result += " | ";
		}
		result += "\n";
	}
	return result;
}
