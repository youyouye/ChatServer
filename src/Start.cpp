/*
 * Start.cpp
 *
 *  Created on: Apr 4, 2017
 *      Author: moocos
 */
/*
#include "ChatServer.h"
#include "inspect/DataInspector.h"
#include <muduo/net/inspect/Inspector.h>
#include <muduo/net/EventLoopThread.h>

int main(int argc, char* argv[])
{
	LOG_INFO << "pid = " << getpid();
    EventLoop loop;

    InetAddress serverAddr(2017);
    ChatServer server(&loop, serverAddr);
    server.setThreadNum(4);
    server.start();

    EventLoopThread t;
    DataInspector ins(t.startLoop(), InetAddress(12345), "test",&server);

    loop.loop();


}
*/




