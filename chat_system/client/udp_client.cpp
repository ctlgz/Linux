#include "udp_client.h"

udp_client::udp_client(const std::string &_ip, const int &_port):
	sock(-1)
{
	server.sin_family = AF_INET;
	server.sin_port = htons(_port);
	server.sin_addr.s_addr = inet_addr(_ip.c_str());
}

int udp_client::initClient()
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		std::cerr << "socket error!" << std::endl;
		return -1;
	}
	return 0;
}

int udp_client::recvData(std::string &outString)
{
	char buf[SIZE];
	struct sockaddr_in peer;
	socklen_t len = sizeof(peer);
	ssize_t s = recvfrom(sock, buf, sizeof(buf)-1, 0,\
			(struct sockaddr*)&peer, &len);
	if(s > 0){
		buf[s] = 0;
		outString = buf;
	}
	return s;
}

int udp_client::sendData(const std::string &inString)
{
	socklen_t len = sizeof(server);
	ssize_t s = sendto(sock, inString.c_str(), inString.size(), 0,\
			(struct sockaddr*)&server, len);
	if(s < 0){
		std::cerr << "sendto" << std::endl;
		return -1;
	}
	return s;
}

udp_client::~udp_client()
{
	if(sock != -1){
		close(sock);
	}
}











