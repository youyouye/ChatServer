#ifndef PREROOM_H_
#define PREROOM_H_

#include <vector>
#include <string>
#include <muduo/net/EventLoop.h>
#include "Player.h"

class PreRoom
{
	public:
		PreRoom();
		~PreRoom();
		init();
	public:
		int getRoomId() const {
			return room_id;
		}
		std::string playerToString() const;
	public:
		void postToPlayers();
		void postTimeToPlayers();
		void okProcess();
		void timerProcess();
		void startGame();
	public:
		void cancleTimer();
		void startTimer();	
	private:
		int maxCount; //总容量
		std::vector<Player> players;	//玩家
		int oknum;	//准备好的人数
		int pre_time;	//时间,为0时开始.
		int room_id;	//房间号.	
		int status; //0表示等待.1表示执行定时器.
	private:
		TimerId* timer_;	
		const EventLoop* loop_;
		const GameData* data_;	
};

#endif
