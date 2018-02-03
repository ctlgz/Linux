#ifndef _UDP_CLIENT_H_
#define _UDP_CLIENT_H_

#include <iostream>
#include <map>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define SIZE 1024

class udp_client{
	public:
		udp_client(const std::string &_ip, const int &_port);
		int initClient();
		int recvData(std::string &outString);
		int sendData(const std::string &inString);
		~udp_client();
	private:
		int sock;
		struct sockaddr_in server;
};

#endif

