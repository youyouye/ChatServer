#include "GameData.h"
#include "AllocateId.h"
#include "GameServer.h"

void GameData::addConnect(TcpConnectionPtr& conn)
{
	int conn_id = AllocateId::allocateConnId();
	conn->setContext(IdInfo(0,conn_id));
	EntryPtr entry(new Entry(conn));
	connections_.insert({conn_id,entry});
}

void GameData::removeConnect(TcpConnectionPtr& conn)
{
}
void GameData::addAccount(TcpConnectionPtr& conn,int account)
{
	IdInfo& info = boost::any_cast<IdInfo>(conn->getContext());
	int conn_id = info.connid_;
	id_connidMap_[account] = conn_id;
}
void GameData::joinRoom(TcpConnectionPtr conn,const int account,const std::string& nickname)
{
	auto it = pre_rooms_.begin();	
	for (;it != pre_rooms_.end();it++)
	{
		if ((*it).players.size() < (*it).maxCount)
		{
			//加入房间
			(*it).players.push_back(Player(conn,account,nickname));
			//回执
			youdrawmeguess::RoomCallback callback;
			callback.set_roomid(room->getRoomId());
			callback.set_playersmess((*it).playerToString());	
			codec_.send(conn,callback);
			//通知其他玩家
			(*it).postOperToPlayers(account,youdrawmeguess::RoomMessType::PJOIN);
			break;
		}
	}
	if (it == pre_rooms_.end())
	{
		shared_ptr<PreRoom> room(new PreRoom()); 
		room->players.push_back(Player(conn,account,nickname));
		pre_rooms_.insert({room->getRoomId(),room});
		//回执房间消息.
		youdrawmeguess::RoomCallback callback;
		callback.set_roomid(room->getRoomId());
		callback.set_playersmess("");	
		codec_.send(conn,callback);
	}
}
void GameData::quitRoom(TcpConnectionPtr conn,int account,int room_id)
{
	if (pre_rooms_.find(room_id) != pre_rooms_.end())
	{
		auto room = pre_rooms_[room_id];
	 	auto target = room.find_if(room->players.begin(),room->players.end(),
				[account](const Player& player){
			return player.getAccount() == account;
		});
		if (target != room.players.end())
		{
			room->postOperToPlayers(account,youdrawmeguess::RoomMessType::PQUIT);
			room.players.erase(target);
		}
	}
	else
	{
		//如果找不到,就说明有问题,给客户端发个错误信息.
	}
}
void GameData::okRoom(TcpConnectionPtr conn,int account,int room_id)
{
	if (pre_rooms_.find(room_id) != pre_rooms_.end())
	{
		auto room = pre_rooms_[room_id];
	 	auto target = room.find_if(room->players.begin(),room->players.end(),
				[account](const Player& player){
			return player.getAccount() == account;
		});
		if (target != room.players.end())
		{
			room->postOperToPlayers(account,youdrawmeguess::RoomMessType::POK);
			(*target).status = 1;
			room->oknum += 1;
			//当一个玩家状态改变时,检测是否开始游戏或者倒计时.
			room->okProcess();	
		}
	}
	else
	{
		//如果找不到,就说明有问题,给客户端发个错误信息.
	}
}

void GameData::allocGameRoom(vector<Player>* players)
{
	auto room = make_shared<GameRoom>(players);
	game_rooms_.insert({room->getRoomId(),room});
	room->postInfoToPlayers();	
}
