#include "mysql_conn.h"
#include "../microhttp/Microhttpd.h"
#include <muduo/net/EventLoop.h>
#include "mysql_conn_pool.h"
using namespace micro;
using namespace muduo;
using namespace muduo::net;
int main()
{
	MysqlConnPool::instance();
	EventLoop loop_;
	MicroHttp http(6668);
	loop_.loop();
}
