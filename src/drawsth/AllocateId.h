#ifndef ALLOCATEID_H_
#define ALLOCATEID_H_
#include <pthread.h>
#include <boost/noncopyable.hpp>
class AllocateId : boost::noncopyable
{
	public:
		static AllocateId& instance()
		{
			pthread_once(&ponce_,&AllocateId::init);
		}
		//FIXME:修改成线程安全
		static int allocatePreRoomId()
		{
			if(pre_roomid_ == 100000)
			{
				pre_roomid_ = 1;
				return pre_roomid_;
			}else{
				pre_roomid_++;
				return pre_roomid_;
			}
		}
		static int allocateConnId()
		{
			conn_id++;
			return conn_id;
		}
		static int allocateGameRoomId()
		{
			game_roomid_++;
			return game_roomid_;			
		}
	private:
		AllocateId();
		~AllocateId();

		static void init()
		{
			value_ = new AllocateId();
		}
		static void destory()
		{
			delete value_;
			value_ = NULL;
		}
	private:
		static pthread_once_t ponce_;
		static AllocateId* value_;
	private:
		static int pre_roomid_;
		static int conn_id;
		static int game_roomid_;
};

pthread_once_t AllocateId::ponce_ = PTHREAD_ONCE_INIT;
AllocateId* AllocateId::value_ = NULL;
int AllocateId::pre_roomid_ = 0;
int AllocateId::conn_id = 0;
int AllocateId::game_roomid_ = 0;
#endif
