/*
 * ChatInspector.cpp
 *
 *  Created on: Apr 13, 2017
 *      Author: moocos
 */

#include "ChatInspector.h"
#include "../server/ChatServer.h"


void ChatInspector::registerCommands(DataInspector* ins)
{
	ins->add("chat","chatserver",boost::bind(&ChatInspector::chatserver, this, _1, _2),"print chatserver");
	ins->add("chat","groupserver",boost::bind(&ChatInspector::groupserver, this, _1, _2),"print groupserver");
}
string ChatInspector::chatserver(HttpRequest::Method, const DataInspector::ArgList&)
{
	  string result;
	  Timestamp now = Timestamp::now();
	  result += "ChatServer at ";
	  result += now.toFormattedString();
	  result += " (UTC)\n ";

	  result += server_->inspector();

	  return result;
}
string ChatInspector::groupserver(HttpRequest::Method, const DataInspector::ArgList&)
{
	  string result;
	  Timestamp now = Timestamp::now();
	  result += "ChatServer at ";
	  result += now.toFormattedString();
	  result += " (UTC)\n ";

	  result += server_->getGroupServer().inspector();

	  return result;
}

