#include <hiredis.h>
#include <string>

class Redis{
	public:
		Redis(const std::string& ip="127.0.0.1",int port=6379,const std::string,const int db=0);
		~Redis();
		std::string get();
		std::string set();
		std::string smembers();
		int del();
		int existsKey()
	private:	
		redisContext* context;
		std::string ip;
		int _port;
		int _db;
		
};
