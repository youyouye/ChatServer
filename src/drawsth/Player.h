#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
class Player{
	public:
		typedef boost::weak_ptr<muduo::net::TcpConnection> WeakTcpConnectionPtr;
	WeakTcpConnectionPtr conn;		
	public:
		int getAccount() const
		{
			return account;
		}
	private:
		int account;
		std::string nickname;
		int status; //准备,默认.
};








#endif

