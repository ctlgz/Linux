#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_

#include <iostream>
#include <map>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "data_pool.h"

#define SIZE 1024
#define PSIZE 128

class udp_server{
	public:
		udp_server(const std::string &_ip, const int &_port);
		int initServer();
		int recvData(std::string &outString);
		int broadcast();
		~udp_server();
	private:
		int sendData(const std::string &inString,\
				struct sockaddr_in &client, socklen_t &len);
		void addUser(struct sockaddr_in &peer, socklen_t &len);
		void delUser(struct sockaddr_in &peer, socklen_t &len);

		std::string ip;
		int port;
		int sock;

		std::map<int, struct sockaddr_in> online_user;
		data_pool pool;
};

#endif









