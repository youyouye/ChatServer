/*
 * online_user.h
 *
 *  Created on: Jun 13, 2017
 *      Author: moocos
 */

#include <map>
#include <pthread.h>
#ifndef ONLINE_USER_H_
#define ONLINE_USER_H_

class OnlineUser{
public:
	OnlineUser();
	~OnlineUser();
	static OnlineUser* instance();
	static void alterUser(int id,int status);
private:
	static void init();
	static std::map<int,int> userStatus;
	static OnlineUser* values_;
	static pthread_once_t ponce_;
};



















#endif /* ONLINE_USER_H_ */
