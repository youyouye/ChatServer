/*
 * mysql_conn_poll.cpp
 *
 *  Created on: Jun 10, 2017
 *      Author: moocos
 */
#include "mysql_conn_pool.h"
#include <mysql.h>
#include <assert.h>
#include <stdio.h>

pthread_once_t MysqlConnPool::ponce_ = PTHREAD_ONCE_INIT;
bool MysqlConnPool::started_ = false;
int MysqlConnPool::numConns_ = 4;
int MysqlConnPool::next_ = 0;
std::vector<MYSQL*> MysqlConnPool::conns_;
std::vector<boost::shared_ptr<MysqlConn> > MysqlConnPool::mysqlConns_;
MysqlConnPool* MysqlConnPool::value_ = NULL;
muduo::MutexLock MysqlConnPool::mutex_;

MysqlConnPool::MysqlConnPool()
{
}
MysqlConnPool::~MysqlConnPool()
{
}
MysqlConnPool* MysqlConnPool::instance()
{
	pthread_once(&ponce_,&MysqlConnPool::init);
	return value_;
}

MysqlConn* MysqlConnPool::getNextConn()
{
	assert(started_);
	MysqlConn* mysql = NULL;
	muduo::MutexLockGuard lock(mutex_);
	if (!mysqlConns_.empty())
	{
		mysql = mysqlConns_[next_].get();
		++next_;
		if (next_ >= mysqlConns_.size()){
			next_ = 0;
		}
	}
	return mysql;
}
MysqlConn* MysqlConnPool::getConnForHash(size_t hashCode)
{
	muduo::MutexLockGuard lock(mutex_);
	MysqlConn* mysql = NULL;
	if (!mysqlConns_.empty()){
		mysql = mysqlConns_[hashCode % mysqlConns_.size()].get();
	}
	return mysql;
}
void MysqlConnPool::init()
{
	assert(!started_);
	started_ = true;
	for (int i = 0;i < numConns_;i++)
	{
		MysqlConn* t = new MysqlConn();
		mysqlConns_.push_back(boost::shared_ptr<MysqlConn>(t));
		conns_.push_back(t->init());
	}
	value_ = new MysqlConnPool();
}


