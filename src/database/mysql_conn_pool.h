/*
 * mysql_conn_pool.h
 *
 *  Created on: Jun 10, 2017
 *      Author: moocos
 */

#ifndef MYSQL_CONN_POOL_H_
#define MYSQL_CONN_POOL_H_

#include <boost/shared_ptr.hpp>
#include <vector>
#include <pthread.h>
#include <muduo/base/Mutex.h>
#include "mysql_conn.h"

typedef struct st_mysql MYSQL;

class MysqlConnPool{
public:
	MysqlConnPool();
	~MysqlConnPool();
	static MysqlConnPool* instance();
	void setConnNum(int numConn){this->numConns_ = numConn;}
	MysqlConn* getNextConn();
	MysqlConn* getConnForHash(size_t hashCode);
	bool started() const
	{ return started_;}

private:
	static void init();
	static bool started_;
	static int numConns_;
	static int next_;
	static std::vector<boost::shared_ptr< MysqlConn > > mysqlConns_;
	static std::vector<MYSQL*> conns_;
	static pthread_once_t ponce_;
	static MysqlConnPool* value_;
	static muduo::MutexLock mutex_;
};














#endif /* MYSQL_CONN_POOL_H_ */
