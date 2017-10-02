#ifndef GAMESERVER_H_
#define GAMESERVER_H_

#include "youdrawmeguess.pb.h"
#include "../server/codec.h"
#include "../server/dispatcher.h"
#include "GameData.h"
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <google/protobuf/message.h>
#include <string>

using namespace muduo;
using namespace muduo::net;

typedef boost::shared_ptr<youdrawmeguess::Connect> ConnectPtr;
typedef boost::shared_ptr<youdrawmeguess::RoomOpera> RoomOperaPtr;
typedef boost::shared_ptr<youdrawmeguess::RoomMessage> RoomMessPtr;
typedef boost::shared_ptr<youdrawmeguess::RoomCallback> RoomCallbackPtr;
typedef boost::shared_ptr<youdrawmeguess::Ack> AckPtr;
class GameServer : boost::noncopyable
{
	public:
		GameServer(EventLoop* loop,
					const InetAddress& listenAddr);
		~GameServer();
		void start();
		void sendMessage(const google::protobuf::Message& mess);
	private:
		void onUnknownMessage(const TcpConnectionPtr& conn,
	                        const MessagePtr& message,
	                        const Timestamp& time);
		void onConnection(const TcpConnectionPtr& conn,
					const std::string& message,
					const Timestamp& time);
		void onConnect(const TcpConnectionPtr& conn,
					const ConnectPtr& message,
					const Timestamp&);
		void onRoomOpera(const TcpConnectionPtr& conn,
						const RoomOperaPtr& message,
						const Timestamp& time);
		void onRoomMess(const TcpConnectionPtr& conn,
						const RoomMessPtr& message,
						const Timestamp& time);
		void onRoomCallback(
						const TcpConnectionPtr& conn,
						const RoomCallbackPtr& message,
						const Timestamp& time);
		void onAck(const TcpConnectionPtr& conn,
				   const RoomCallbackPtr& message,
				   const Timestamp& time);
	private:
		TcpServer server_;
		ProtobufCodec codec_;
		ProtobufDispatcher dispatcher_;
	public:
		GameData* gamedata_;
	private:

};




#endif
