#include <string>
#include <vector>
#include <hiredis/hiredis.h>
#include <hash_map>
using namespace __gnu_cxx;
class Redis{
	public:
		Redis(const std::string& ip="127.0.0.1",int port=6379,const std::string& ="",const int db=0);
		~Redis();
	   	bool connect();
		bool get(const std::string& key,std::string& val);      //获取指定key的val                                  
		bool get(const std::vector<std::string>& keys,std::vector<std::string>& vals);      //获取多个key的val
		bool get(const std::vector<std::string>& keys,hash_map<std::string,std::string>& vals);
		bool hget(const std::string& hkey,const std::vector<std::string>& keys,std::vector<std::string>& vals);     
		bool set(const std::string& key,const std::string& val);
		bool set_old(const std::string& key,const std::string& val);
		bool keys(const std::string& key,std::vector<std::string> valkeys);
		bool del(const std::string& key);
		bool set_expire(const std::string& key,size_t expire_seconds);
		redisContext* select_db(redisContext *context);

	private:	
		redisContext* _context;
		std::string _ip;
		int _port;
		int _db;
		std::string _passwd;
};
