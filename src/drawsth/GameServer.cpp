#include "GameServer.h"
#include <muduo/base/Logging.h>
#include <boost/bind.hpp>
#include <string>

GameServer::GameServer(EventLoop* loop,
			const InetAddress& listenAddr)
	:server_(loop,listenAddr,"GameServer",TcpServer::kReusePort),
	dispatcher_(boost::bind(&GameServer::onUnknownMessage,this,_1,_2,_3)),
	codec_(boost::bind(&ProtobufDispatcher::onProtobufMessage, &dispatcher_, _1, _2, _3))
{
	dispatcher_.registerMessageCallback<youdrawmeguess::Connect>(boost::bind(&GameServer::onConnect,this,_1,_2,_3));	
	dispatcher_.registerMessageCallback<youdrawmeguess::RoomOpera>(boost::bind(&GameServer::onRoomOpera,this,_1,_2,_3));
	dispatcher_.registerMessageCallback<youdrawmeguess::RoomMessage>(boost::bind(&GameServer::onRoomMess,this,_1,_2,_3));

	server_.setConnectionCallback(boost::bind(&GameServer::onConnection,this,_1,_2,_3));
	server_.setMessageCallback(boost::bind(&ProtobufCodec::onMessage,&codec_,_1,_2,_3));
}
void GameServer::start()
{
	server_.start();
}

void GameServer::sendMessage(const google::protobuf::Message& mess)
{
}	

void GameServer::onConnection(const TcpConnectionPtr& conn,
					const std::string& message,
					const Timestamp& time)
{
	LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");
	//先不加锁,试一下无锁机制好了..
	
	if (conn->connected())
	{
		gamedata_->addConnect(conn);
	}
	else
	{
		gamedata_->removeConnect(conn);
	}

}
void GameServer::onConnect(const TcpConnectionPtr& conn,
				const ConnectPtr& message,
				const Timestamp&)
{
	LOG_INFO << message->id() << "is conn.";
	gamedata_->addAccout(conn,message->id());
}
void GameServer::onRoomOpera(const TcpConnectionPtr& conn,
				const RoomOperaPtr& message,
				const Timestamp& time)
{
	if (message->type() == youdrawmeguess::RoomOpera::JOIN)
	{
		gamedata_->joinRoom(conn,"","");		
	}
	else if (message->type() == youdrawmeguess::RoomOpera::QUIT)
	{
		gamedata_->quitRoom();
	}
	else if (message->type() == youdrawmeguess::RoomOpera::OK)
	{
		gamedata_->okRoom();
	}

}


void GameServer::onRoomMess(const TcpConnectionPtr& conn,
				const RoomMessPtr& message,
				const Timestamp& time)
{
	
}

void GameServer::onRoomCallback(
			const TcpConnectionPtr& conn,
			const RoomCallbackPtr& message,
			const Timestamp& time)
{
	
}
void GameServer::onAck(const TcpConnectionPtr& conn,
		 const RoomCallbackPtr& message,
		 const Timestamp& time)
{
	
}


void GameServer::onUnknownMessage(const TcpConnectionPtr& conn,
                        const MessagePtr& message,
                        Timestamp)
{
	LOG_INFO << "onUnknownMessage: " << message->GetTypeName();
	    conn->shutdown();
}




