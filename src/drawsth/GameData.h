#ifndef GAMESERVERDATA_H_
#define GAMESERVERDATA_H_

#include <muduo/net/TcpConnection.h>
#include <hash_map>
#include <map>
#include <vector>
#include <memory>
#include <string>
#include "GameRoom.h"
#include "PreRoom.h"
using namespace std;
using namespace __gnu_cxx;
class GameData
{
	public:
		void addConnect(TcpConnectionPtr& conn);
		void removeConnect(TcpConnectionPtr& conn);
		void addAccount(TcpConnectionPtr& conn,int id);
	public:
		void joinRoom(TcpConnectionPtr conn,int account,const string& nickname);
		void quitRoom();
		void okRoom();
	public:
		void allocGameRoom(vector<Player>* players);
	public:
		struct Entry : public muduo::copyable
		{
			typedef boost::weak_ptr<muduo::net::TcpConnection> WeakTcpConnectionPtr;	
			explicit Entry(const WeakTcpConnectionPtr& weakConn)
	    	  : weakConn_(weakConn)
	   		{
	   		}
	   		
			~Entry()
	   		{
				muduo::net::TcpConnectionPtr conn = weakConn_.lock();
	       		if (conn)
	        	{
	        		conn->shutdown();
	      		}
	    	}
		  	WeakTcpConnectionPtr weakConn_;
		};
		
		typedef boost::shared_ptr<Entry> EntryPtr;
		typedef boost::weak_ptr<Entry> WeakEntryPtr;
		typedef hash_map<int,EntryPtr> ConnectionList;
		typedef hash_map<int,int> Id_ConnId;

		typedef boost::shared_ptr<ConnectionList> ConnectionListPtr;
		typedef boost::shared_ptr<Id_ConnId> Id_ConnIdPtr;

		ConnectionListPtr connections_;
		Id_ConnIdPtr id_connidMap_;

	public:
		hash_map<int,shared_ptr<PreRoom>> pre_rooms_;
		hash_map<int,shared_ptr<GameRoom>> game_rooms_;
	private:
		class IdInfo{
			IdInfo(int connid,int accid)
			{
				connid_ = connid;
				accid_ = accid;
			}
			int connid_;
			int accid_;
		}		
};



#endif
