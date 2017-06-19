/*
 * Start.cpp
 *
 *  Created on: Apr 4, 2017
 *      Author: moocos
 */

#include "ChatServer.h"
#include "inspect/DataInspector.h"
#include <muduo/net/inspect/Inspector.h>
#include <muduo/net/EventLoopThread.h>
#include "database/mysql_conn_pool.h"
#include "microhttp/Microhttpd.h"
using namespace micro;

int main(int argc, char* argv[])
{
	LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    //HTTP服务器
    MysqlConnPool::instance();
	MicroHttp http(6666);

    InetAddress serverAddr(2017);
    ChatServer server(&loop, serverAddr);
    server.setThreadNum(4);
    server.start();

    EventLoopThread t;
    DataInspector ins(t.startLoop(), InetAddress(12345), "test",&server);
    EventLoopThread t1;
    Inspector spector(t1.startLoop(),InetAddress(12346),"inspect");
    loop.loop();
}




