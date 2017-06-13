/*
 * mysql_conn.cpp
 *
 *  Created on: Jun 9, 2017
 *      Author: moocos
 */

#include "mysql_conn.h"
#include <mysql.h>
#include <my_global.h>
#include <muduo/base/Logging.h>

MysqlConn::MysqlConn()
	:conn(NULL),exiting_(false),mutex_()
{
}
MysqlConn::~MysqlConn()
{
	exiting_ = true;
	mysql_close(conn);
}

MYSQL* MysqlConn::init(){
	if ((conn = mysql_init(NULL)) == NULL)
	{
		printf("mysql初始化失败");
		return NULL;
	}
	if (mysql_real_connect(conn,
			"127.0.0.1","root","root",
			"im",0,NULL,0) == NULL)
	{
	    fprintf(stderr,"%s:\nError %u (%s)\n","mysql_real_connect 失败:\nError %u (%s)\n",mysql_errno(conn),mysql_error(conn));
		return NULL;
	}
	return conn;
}
int MysqlConn::login(std::string account,std::string password)
{
	muduo::MutexLockGuard lock(mutex_);
	char *select1;
	select1 = malloc(100);
	char *select = "select * from User where account = '%s' and password = '%s'";
	snprintf(select1,100,select,account.c_str(),password.c_str());
	if(!mysql_query(conn,select1)){
		if (process_get_result_num(conn) > 0){
			return 1; //查询成功
		}else{
			return -1;//查询不到
		}
	}else{
		return -3;//内部错误
	}
}
int MysqlConn::regis(std::string account,std::string password)
{
	muduo::MutexLockGuard lock(mutex_);
	char * select ;
	sprintf(select,"select * from User where account = '%s' and password = '%s'",account.c_str(),password.c_str());

	if(mysql_query(conn,select)){
		if(process_get_result_num(conn) > 0){
			return -1; //已存在
		}else{
			char * insert;
			sprintf(insert,"insert into User(uid,account,password) values ('%s','%s','%s')",account.c_str(),account.c_str(),password.c_str());
			if (mysql_query(conn,insert)){
				return -2; //插入失败
			}else{
				return 1;//插入成功
			}
		}
	}else{
		return -3; //内部错误.
	}

}
int MysqlConn::addfrined(std::string uid)
{
	muduo::MutexLockGuard lock(mutex_);
	return 0;
}
std::string MysqlConn::process_result_set(MYSQL* connect)
{
	return "";
}
int MysqlConn::process_get_result_num(MYSQL* connect)
{
	MYSQL_RES* result = mysql_store_result(connect);
	int num = result->row_count;
	return num;
}

