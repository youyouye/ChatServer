#ifndef MYSQL_CONN_H
#define MYSQL_CONN_H
#include <pthread.h>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <muduo/base/Mutex.h>
#include "offline_msg.h"

typedef struct st_mysql MYSQL;
class MysqlConn
{
public:
	MysqlConn();
	~MysqlConn();
	int login(std::string account,std::string password);
	int regis(std::string account,std::string password);
	int addfrined(std::string uid);
	int addSingleOfflineMsg(SingleOfflineMsg sm);
	typedef boost::shared_ptr<std::vector<SingleOfflineMsg> > SingleOffMsgListPtr;
	int getSingleOffLineMsg(int page,int uid,SingleOffMsgListPtr offmsg);
	MYSQL* init();
private:
	std::string process_result_set(MYSQL* res);
	int process_get_result_num(MYSQL* res);
	MYSQL* conn;
	bool exiting_;
	muduo::MutexLock mutex_;
};


#endif
