/*
 * JsonUtil.cpp
 *
 *  Created on: Jun 12, 2017
 *      Author: moocos
 */
#include <json/json.h>
#include "JsonUtil.h"
std::string JsonUtil::generateResult(int error)
{
	Json::Value root;
	Json::FastWriter writer;
	root["result"] = error;
	std::string res = writer.write(root);
	return res;
}




