/*
 * online_user.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: moocos
 */
#include "online_user.h"

std::map<int,int> OnlineUser::userStatus;
pthread_once_t OnlineUser::ponce_ = PTHREAD_ONCE_INIT;
OnlineUser* OnlineUser::values_ = NULL;

OnlineUser::OnlineUser()
{
}
OnlineUser::~OnlineUser()
{
	delete values_;
}

OnlineUser* OnlineUser::instance()
{
	pthread_once(&ponce_,&OnlineUser::init);
	return values_;
}

void OnlineUser::alterUser(int id,int status)
{
	userStatus[id] = status;
}
void OnlineUser::init()
{
	values_ = new OnlineUser();
}


