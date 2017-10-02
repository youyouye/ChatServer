#include "MessAcker.h"
#include <algorithm>
Mess::Mess()
	:expiredTime_(0),
	retryCount_(2),
	retryTimes_(0),
	type_(1)
{
}

Mess::Mess(google::protobuf::Message* mess,std::string id)
	:expiredTime_(0),
	retryCount_(2),
	retryTimes_(0),
	type_(1),
	id_(id),
	message_(mess)
{
}

Mess::~Mess()
{
	//send mess type
	if (type == 1){
		sendFunc->normalCancleTimer(this);
	}
}

MessPool::MessPool(EventLoop* loop,ProtobufCodec* codec)
	:is_turn_front(true)
	,loop_(loop),codec_(codec)
{
	front_recv_mess.reserve(100);
	back_recv_mess.reserve(100);
	init();	
}

void MessPool::init()
{
	normal_send_func->bindTimer = std::bind(&MessPool::normalBindTimer,this);
	normal_send_func->cancleTimer = std::bind(&MessPool::normalCancleTimer,this);
	normal_send_func->reSend = std::bind(&MessPool::normalReSend,this);
	normal_send_func->delMess = std::bind(&MessPool::normalDelMess,this);
	normal_send_func->expired = std::bind(&MessPool::normalExpired,this);
	normal_recv_func->sendAck = std::bind(&MessPool::normalSendAck,this);
}

void MessPool::saveRecvMess(shared_ptr<Mess> mess)
{
	if (is_turn_front)
	{
		if (front_recv_mess.size() == 100)
		{
			is_turn_front = false;
			if (back_recv_mess.size() == 0){
				back_recv_mess.push_back(mess);
			}else{
				back_recv_mess.clear();
				back_recv_mess.push_back(mess);
			}
		}	
	}
	else
	{
		if (back_recv_mess.size() == 100)
		{
			is_turn_front = true;
			front_recv_mess.clear();
			front_recv_mess.push_back(mess);
		}
	}
}

bool MessPool::findRecvMess(shared_ptr<Mess> mess)
{
	bool find = false;
	auto it = std::find_if(front_recv_mess.begin(),front_recv_mess.end(),
		[mess](shared_ptr<Mess> item)
			{
				return item->id_ = mess->id_;
			});
	if (it != front_recv_mess.end())
	{
		find = true;
	}
	if (!find){
		auto it = std::find_if(back_recv_mess.begin(),back_recv_mess.end(),
			[mess](shared_ptr<Mess> item)
				{
					return item->id_ = mess->id_;
				});
		if (it != back_recv_mess.end())
		{
			find = true;
		}
	}
	return find;
}

void MessPool::normalBindTimer(const Mess& mess)
{
	TimerId timer = loop_->runEvery(2,std::bind(&MessPool::normalReSend,this,mess));	
	mess.timer = timer;
}
void MessPool::normalCancleTimer(const Mess& mess)
{
	Timer timer = mess.timer;
	loop_.cancle(timer);
}
void MessPool::normalReSend(const Mess& mess)
{
	if (mess.retryTimes >= mess.retryCount)
	{
		if (mess.conn->lock())
		{
			//FIXME:can?
			if (conn->connected()){
				conn->shutdown();
			}
		}
		//FIXME:can?
		mess.sendFunc->normalDelMess(mess);
	}
	mess.retryTimes += 1;
	if (mess.conn->lock())
	{
		codec_->send(mess.conn,mess.message);
	}
}
void MessPool::normalDelMess(const Mess& mess)
{
	auto it = send_mess_map.find(mess.id);
	if (it != send_mess_map.end()){
		send_mess_map.erase(it);
	}
}

void MessPool::normalSendAck(const Mess& mess)
{
	
}

MessAcker::MessAcker(EventLoop* loop,ProtobufCodec* codec)
	:loop_(loop),codec_(codec),messPool_(loop,codec)
{
}
MessAcker::~MessAcker()
{
}

void MessAcker::sendMess(const string& id,const google::protobuf::Message* mess)
{
	auto message = make_shared<Mess>(mess,id);
	messPool_.send_mess_map.insert(pair<string,shared_ptr<Mess>>(id,mess));	
	message->sendFunc->normalBindTimer(*mess);
}
void MessAcker::recvMess(const string& id,const google::protobuf::Message* mess)
{
	auto message = make_shared<Mess>(mess,id);
	if (!messPool_.findRecvMess(message))
	{
		messPool_.saveRecvMess(message);
	}
	message->recvFunc->normalSendAck();
}
