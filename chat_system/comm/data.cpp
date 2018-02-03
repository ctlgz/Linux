#include "data.h"

data::data()
{
}

void data::dataSerialize(std::string &outString)
{
	Json::Value root;
	root["nick_name"] = nick_name;
	root["school"] = school;
	root["msg"] = msg;
	root["cmd"] = cmd;
	serialize(root, outString);
}

void data::dataUnserialize(std::string &inString)
{
	Json::Value root;
	unserialize(inString, root);
	nick_name = root["nick_name"].asString();
	school = root["school"].asString();
	msg = root["msg"].asString();
	cmd = root["cmd"].asString();
}

data::~data()
{
}


//int main()
//{
//	data d;
//	d.nick_name = "flypig";
//	d.school = "bit";
//	d.msg = "hello world";
//	d.cmd = "";
//	std::string sendString;
//	d.dataSerialize(sendString);
//	std::cout << sendString << std::endl;
//	return 0;
//}








