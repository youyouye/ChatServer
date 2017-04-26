/*
 * ChatInspector.h
 *
 *  Created on: Apr 13, 2017
 *      Author: moocos
 */

#ifndef CHATINSPECTOR_H_
#define CHATINSPECTOR_H_
#include "DataInspector.h"
#include <boost/noncopyable.hpp>

class ChatServer;

namespace muduo{
namespace net{

class ChatInspector : boost::noncopyable{
public:
	ChatInspector(ChatServer* cs)
	:server_(cs)
	{
	}
	void registerCommands(DataInspector* ins);
	string chatserver(HttpRequest::Method, const DataInspector::ArgList&);
	string groupserver(HttpRequest::Method, const DataInspector::ArgList&);

	::ChatServer* server_;
};

}
}



#endif /* CHATINSPECTOR_H_ */
