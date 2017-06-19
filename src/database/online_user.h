/*
 * online_user.h
 *
 *  Created on: Jun 13, 2017
 *      Author: moocos
 */
#ifndef ONLINE_USER_H_
#define ONLINE_USER_H_

#include <hash_map>
#include <pthread.h>
#include <muduo/base/Mutex.h>
#include <boost/shared_ptr.hpp>
using namespace __gnu_cxx;

class OnlineUser{
public:
	OnlineUser();
	~OnlineUser();
	static OnlineUser* instance();
	static void alterUser(int id,int status);
	static int getStatus(int id);
private:
	typedef hash_map<int,int> UserStatusList;
	typedef boost::shared_ptr<UserStatusList> UserStatusListPtr;
	static void init();
	static UserStatusListPtr getUserStatus(){
		muduo::MutexLockGuard lock(mutex_);
		return userStatus;
	}
	static UserStatusListPtr userStatus;
	static OnlineUser* values_;
	static pthread_once_t ponce_;
	static muduo::MutexLock mutex_;
};



















#endif /* ONLINE_USER_H_ */
