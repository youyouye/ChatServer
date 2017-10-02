#include "redis_conn.h"
using namespace std;
//暂时只写需要的函数.
Redis::Redis(const std::string& ip="127.0.0.1",int port=6379,const std::string& passwd,const int db=0)
{
	_ip = ip;
	_port = port;
	_passwd = passwd;
	_db = db;
}

Redis::~Redis()
{
}

bool Redis::connect()
{
	if (_context)
	{
		redisFree(_context);
		_context = NULL;
	}
	_context = redisConnect(_ip.c_str(),_port);
	if (_context == NULL)
	{
			return false;
	}
	if (_context->err)
	{
		redisFree(_context);	
		_context = NULL;
		return false;
	}
	if ("" != _passwd)
	{
		redisReply* reply = redisCommand(_context,"AUTH %s",_passwd.c_str());
		if (reply == NULL || reply->type == REDIS_REPLY_ERROR)
		{
				return false;
		}
	}
	_context = select_db(_context);
	return true;
}

redisContext* Redis::select_db(redisContext *context)
{
	redisReply *reply;
	reply = (redisReply*)redisCommand(context,"SELECT %d",_db);
	if (reply != NULL)
	{
		freeReplyObject(reply);
	}	
	return context;
}

bool Redis::get(const std::string& key,std::string& val)
{
}                                     
bool Redis::get(const std::vector<std::string>& keys,std::vector<std::string>& vals)
{
}
bool Redis::get(const std::vector<std::string>& keys,__gnu_cxx::hash_map<std::string,std::string>& vals)
{
}
bool Redis::hget(const std::string& hkey,const std::vector<std::string>& keys,std::vector<std::string>& vals)
{
}
bool Redis::set(const std::string& key,const std::string& val)
{
}
bool Redis::set_old(const std::string& key,const std::string& val)
{
}
bool Redis::keys(const std::string& key,std::vector<std::string> valkeys)
{
}
bool Redis::del(const std::string& key)
{
}
bool Redis::set_expire(const std::string& key,size_t expire_seconds)
{

}


