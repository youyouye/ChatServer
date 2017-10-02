#ifndef GAMEROOM_H_
#define GAMEROOM_H_

#include "Player.h"
#include <vector>

class GameRoom
{
	public:
		GameRoom(std::vector<Player>* players);
		void init();
	public:
		int getRoomId() const
		{
			return room_id;
		}
	public:
		void postInfoToPlayers();
	private:
		const std::vector<Player>* players_;
		int num_;
		int room_id_;
};





#endif
