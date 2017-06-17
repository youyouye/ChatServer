/*
 * offline_msg.h
 *
 *  Created on: Jun 13, 2017
 *      Author: moocos
 */

#ifndef OFFLINE_MSG_H_
#define OFFLINE_MSG_H_
#include <string>

class SingleOfflineMsg{
public:
	SingleOfflineMsg();
	SingleOfflineMsg(int ru,int mi,int su,int mt,std::string mc);
	~SingleOfflineMsg();
public:
	int receive_uid;
	int msg_id;
	int send_uid;
	int msg_type;
	std::string msg_content;
};

class GroupOfflineMsg{
public:
	GroupOfflineMsg();
	~GroupOfflineMsg();
};

class FriendOffMsg{
public:
	FriendOffMsg();
	FriendOffMsg(int aid,int rid,std::string ac);
	~FriendOffMsg();
public:
	int askid;
	int reqid;
	std::string ask_content;
};

class FriendRlyMsg{
public:
	FriendRlyMsg();
	FriendRlyMsg(int si,int ri,int ry);
	~FriendRlyMsg();
public:
	int sendid;
	int recvid;
	int reply;
};








#endif /* OFFLINE_MSG_H_ */
