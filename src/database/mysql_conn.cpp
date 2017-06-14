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

	if(!mysql_query(conn,select)){
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

int MysqlConn::addSingleOfflineMsg(SingleOfflineMsg sm)
{
	muduo::MutexLockGuard lock(mutex_);
	char *select1;
	select1 = malloc(1000);
	char *select = "insert into OffineMsg(receiver_uid,msg_id,send_uid,msg_type,msg_content) values (%d,%d,%d,%d,'%s')";
	snprintf(select1,1000,select,sm.receive_uid,sm.msg_id,sm.send_uid,sm.msg_type,sm.msg_content.c_str());
	if (mysql_query(conn,select1)){
		return -2; //插入失败
	}else{
		return 1;//插入成功
	}
}
int MysqlConn::getSingleOffLineMsg(int page,int uid,SingleOffMsgListPtr msgPtr)
{
	muduo::MutexLockGuard lock(mutex_);
	if (page > 0){
		char *del1;
		del1 = malloc(1000);
		char *del = "delete from OffineMsg where receiver_uid = '%s' limit 0,50";
		snprintf(del1,1000,del,uid);
		if (mysql_query(conn,del)){
			LOG_ERROR<<"删除语句执行失败";
		    fprintf(stderr,"%s:\nError %u (%s)\n","mysql_real_connect 失败:\nError %u (%s)\n",mysql_errno(conn),mysql_error(conn));
			return -1; //查询失败
		}
	}
	char *select1;
	select1 = malloc(1000);
	char *select = "select * from OffineMsg where receiver_uid = '%s' limit %d,%d";
	snprintf(select1,1000,select,uid,0,50);
	if (mysql_query(conn,select1)){
		LOG_ERROR<<"查询语句执行失败";
	    fprintf(stderr,"%s:\nError %u (%s)\n","mysql_real_connect 失败:\nError %u (%s)\n",mysql_errno(conn),mysql_error(conn));
		return -1; //查询失败
	}
	MYSQL_RES* res_set = mysql_store_result(conn);
	if (res_set == NULL){
	    fprintf(stderr,"%s:\nError %u (%s)\n","mysql_real_connect 失败:\nError %u (%s)\n",mysql_errno(conn),mysql_error(conn));
	    return -2; //结果集问题
	}else{
		int num = res_set->row_count;
		if (num == 0){
			mysql_free_result(res_set);
			return 1;
		}else{
			MYSQL_ROW row;
			while ( (row=mysql_fetch_row(res_set))!=NULL ){
				SingleOfflineMsg offmsg;
				LOG_INFO<<"离线信息列数:"<<mysql_num_fields(res_set);
				offmsg.receive_uid = (row[2]!=NULL ? atoi(row[2]):-1);
				offmsg.msg_id = (row[3]!=NULL ? atoi(row[3]):-1);
				offmsg.send_uid = (row[5]!=NULL ? atoi(row[5]):-1);
				offmsg.msg_type = (row[6]!=NULL ? atoi(row[6]):-1);
				offmsg.msg_content = (row[7]!=NULL ? std::string(row[7]): "");
				msgPtr->push_back(offmsg);
			}
		}
		mysql_free_result(res_set);
		return 1;
	}
}
