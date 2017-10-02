#include "PreRoom.h"
#include "AllocateId.h"
PreRoom::PreRoom()
	:maxCount(6),
	oknum(0),
	pre_time(20),
	room_id(0)
{
	init();
}
PreRoom::~PreRoom()
{
}
PreRoom::init()
{
	room_id = AllocateId::instance()->allocatePreRoomId();
}	

std::string PreRoom::playerToString()
{
	std::string res="";
	for (auto it = players.begin();it != players.end();it++)
	{
		res += (*it).account+"_;
		res += (*it).nickname+"|";
	}
	return res;
}

void PreRoom::postOperToPlayers(int account,const youdrawmeguess::RoomMessType& type)
{
	for (auto it = players.begin();it != players.end();it++)
   	{
		//不给操作对象发.
		if ((*it).account == account)
		{
			continue;
		}
		if ((*it).conn.lock())
		{
			youdrawmeguess::RoomMessage mess;
			mess.type = type;
			mess.message(itoa(account));
			codec_.send((*it).conn,mess);
		}	
	}
}

void PreRoom::okProcess()
{
	if (oknum < 3)
	{
		if (status == 1)
		{
			//取消定时器.
			cancleTimer();
		}	
	}	
	if (oknum == maxCount)
	{
		//开始游戏.创建房间?我还是想分开进入和游戏,处理起来方便点.
		//变量可以少点,分两处处理逻辑.
		startGame();
	}
	else if (oknum >= 3)
	{
		if (status == 0)
		{
			startTimer();
		}
	}
}

void PreRoom::timerProcess()
{
	if (status == 0)
		return;
	pre_time = pre_time - 1;
	postTimeToPlayers();
	if (pre_time == 0)
	{
		//start_game;
		cancleTimer();
		startGame();
	}	
}

void PreRoom::postTimeToPlayers()
{
	for (auto it = players.begin();it != players.end();it++)
   	{
		//FIXME:如果掉线了怎么办?或者强退了,这里需要处理.删除玩家吧..
		if ((*it).conn.lock())
		{
			youdrawmeguess::RoomMessage mess;
			mess.type = youdrawmeguess::RoomMessType::PTIME;
			mess.message(itoa(pre_time));
			codec_.send((*it).conn,mess);
		}	
	}
}

void PreRoom::startGame()
{
	//需要的是玩家,转到让GameData解决吧.要删除房间吗?不要吧,我又想了想,游戏已经开始,房间保留好了.
	data_->allocGameRoom(&players);			
}

void PreRoom::cancleTimer()
{
	status = 0;
	pre_time = 20;//重置
	loop_->cancel(timer_);
}

void PreRoom::startTimer()
{
	status = 1;
	pre_time = 20;
	timer_ = loop_->runEvery(1.0,boost::bind(&PreRoom::timerProcess,this));
}

