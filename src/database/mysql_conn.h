#ifndef MYSQL_CONN_H
#define MYSQL_CONN_H
#include <pthread.h>
#include <string>
#include <muduo/base/Mutex.h>

typedef struct st_mysql MYSQL;
class MysqlConn
{
public:
	MysqlConn();
	~MysqlConn();
	int login(std::string account,std::string password);
	int regis(std::string account,std::string password);
	int addfrined(std::string uid);
	MYSQL* init();
private:
	std::string process_result_set(MYSQL* res);
	int process_get_result_num(MYSQL* res);
	MYSQL* conn;
	bool exiting_;
	muduo::MutexLock mutex_;
};


#endif
