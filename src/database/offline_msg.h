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












#endif /* OFFLINE_MSG_H_ */
