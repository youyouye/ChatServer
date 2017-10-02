#ifndef MESSACKER_H_
#define MESSACKER_H_
#include <functional>
#include <hash_map>
#include <memory>
#include <muduo/net/TimerId.h>
#include <muduo/net/EventLoop.h>
#include <google/protobuf/message.h>

class SendMessFunc
{
	std::function<int(int)> bindTimer;
	std::function<int(int)> cancleTimer;	
	std::function<int(int)> reSend;
	std::function<int(int)> delMess;
};
class RecvMessFunc
{
	std::function<int(int)> sendAck;
};
class Mess
{
	public:
		Mess();
		Mess(google::protobuf::Message* mess,std::string id);
		~Mess();
	private:
		int expiredTime_;
		int retryCount_;
		int retryTimes_;
		int type_;
		TimerId timer_;
		std::string id_;
	private:
		google::protobuf::Message* message_;
	public:
		typedef boost::weak_ptr<TcpConnection> WeakTcpConnectionPtr;
		WeakTcpConnectionPtr conn_;
	public:
		SendMessFunc* sendFunc;
		RecvMessFunc* recvFunc;
};

class MessPool
{
	public:
		MessPool(EventLoop* loop,ProtobufCodec* codec);
		void init();
	public:
		void saveRecvMess(std::shared_ptr<Mess> mess);
		bool findRecvMess(std::shared_ptr<Mess> mess);
	public:
		void normalBindTimer();
		void normalCancleTimer();
		void normalReSend();
		void normalDelMess();
		void normalSendAck();
	private:
		SendMessFunc* normal_send_func;
		SendMessFunc* normal_recv_func;
	public:
		typedef std::hash_map<std::string,std::shared_ptr<Mess>> IdMessMap;
		typedef std::vector<std::shared_ptr<Mess>> MessQueue;
		IdMessMap send_mess_map;
		//为了去重,保留最近的100条消息.2个都填满后,清空某一个.
	private:
		MessQueue front_recv_mess;
		MessQueue back_recv_mess;
		bool is_turn_front;
	private:
		EventLoop* loop_;
		ProtobufCodec* codec_;
};	
//对外提供的函数调用流程.
class MessAcker
{
	public:
		MessAcker(EventLoop* loop,ProtobufCodec* codec);
		~MessAcker();
	public:
		void sendMess(const google::protobuf::Message* mess);
		void recvMess(const google::protobuf::Message* mess);
	private:
		EventLoop* loop_;
		ProtobufCodec* codec_;	
		MessPool messPool_;
};


#endif
