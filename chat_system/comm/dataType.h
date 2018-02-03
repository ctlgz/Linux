#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_

#include <iostream>
#include <string>
#include "jsonApi.h"

#define SIZE 1024

class dataType{
	public:
        int stringToValue(std::string &inString);
        int valueToString(std::string &outString);
	public:
		std::string nick_name;
		std::string school;
		std::string msg;
		std::string cmd;
};

#endif
