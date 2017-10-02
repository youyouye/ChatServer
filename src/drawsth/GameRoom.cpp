#include "GameRoom.h"
#include "AllocateId.h"

GameRoom::GameRoom(std::vector<Player>* players)
	:players_(players),num_(0),room_id_(0)
{
	init();
}

void GameRoom::init()
{
	num_ = players_->size();
	room_id_ = AllocateId::allocateGameRoomId();
}

void GameRoom::postInfoToPlayers()
{
		
}

