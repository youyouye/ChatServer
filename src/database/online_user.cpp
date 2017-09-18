/*
 * online_user.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: moocos
 */
#include "online_user.h"

pthread_once_t OnlineUser::ponce_ = PTHREAD_ONCE_INIT;
OnlineUser* OnlineUser::values_ = NULL;
muduo::MutexLock OnlineUser::mutex_;
OnlineUser::UserStatusListPtr OnlineUser::userStatus(new std::map<int,int>());

OnlineUser::OnlineUser()
{
}
OnlineUser::~OnlineUser()
{
}

OnlineUser* OnlineUser::instance()
{
	pthread_once(&ponce_,&OnlineUser::init);
	return values_;
}

void OnlineUser::alterUser(int id,int status)
{
    muduo::MutexLockGuard lock(mutex_);
	if (!userStatus.unique()){
		userStatus.reset(new UserStatusList(*userStatus));
	}
	(*userStatus)[id] = status;
}
int OnlineUser::getStatus(int id)
{
	UserStatusListPtr ptr = getUserStatus();
	return (*ptr)[id];
}

void OnlineUser::init()
{
	values_ = new OnlineUser();
}


