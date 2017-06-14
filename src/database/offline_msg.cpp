/*
 * offline_msg.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: moocos
 */

#include "offline_msg.h"

SingleOfflineMsg::SingleOfflineMsg()
	:receive_uid(0),
	 msg_id(0),
	 send_uid(0),
	 msg_type(0),
	 msg_content("")
{
}


SingleOfflineMsg::SingleOfflineMsg(int ru,int mi,int su,int mt,std::string mc)
{
	this->receive_uid = ru;
	this->msg_id = mi;
	this->send_uid = su;
	this->msg_type = mt;
	this->msg_content = mc;
}
SingleOfflineMsg::~SingleOfflineMsg()
{
}

GroupOfflineMsg::GroupOfflineMsg()
{

}
GroupOfflineMsg::~GroupOfflineMsg()
{

}


