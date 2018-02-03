#include <signal.h>
#include "udpClient.h"
#include "dataType.h"
#include "window.h"

using namespace std;

static void usage(std::string proc)
{
	cout<<"Usage: "<<proc<<" [remote_ip] [remote_port]"<<endl;
}

window win;
std::string nick_name;
std::string school;
dataType sendData;
dataType recvData;
udpClient *sig_cli = NULL;


static void* run_header(void *arg)
{
	std::string msg = "Welcome To Chat System!";
	int step = 1;
	while(1){
		win.createHeader();
		int h, w;
	    getmaxyx(win.header, h, w);
	    win.putStringToWin(win.header, h/2, step++, msg);
	    win.refreshMyWindow(win.header);
		win.clrNumsLines(win.header, h/2, 1);
		step %= w;
		step = (step==0?1:step);
		usleep(300000);

	//	std::string str;
	//	win.getStringFromWin(win.input, str);

	//    getmaxyx(win.output, h, w);
	//    win.putStringToWin(win.output, 1, 1, str);
	//    win.refreshMyWindow(win.output);
	}
}
static void* run_output_friend(void *arg)
{
	udpClient *_cli = (udpClient*)arg;
	int point = 1;
	win.createOutput();
	win.createFriendList();
	for(;;){
		int fpoint = 1;
	    win.refreshMyWindow(win.output);
	    win.refreshMyWindow(win.friend_list);

		std::string str;
		_cli->recvData(str);
		recvData.stringToValue(str);//
		//zhangsan-SUST# nihao?
		std::string tips = recvData.nick_name;
		tips += "-";
		tips += recvData.school;

		//zhangsan-SUST
		std::string newFriend=tips;
		int h, w;

		if(recvData.cmd == "QUIT"){
			_cli->delUser(newFriend);
		}else{
		    tips += "# ";
		    tips += recvData.msg;

	        getmaxyx(win.output, h, w);
	        win.putStringToWin(win.output, point++, 1, tips);
			_cli->addUser(newFriend);
		}

	    getmaxyx(win.friend_list, h, w);
		win.clrNumsLines(win.friend_list, 1, h-1);
		win.createFriendList();

		std::vector<std::string>::iterator iter = (_cli->flist).begin();

		for(; iter != (_cli->flist).end(); iter++){
			win.putStringToWin(win.friend_list, fpoint++, 1, *iter);
		}

	    win.refreshMyWindow(win.output);
	    win.refreshMyWindow(win.friend_list);

		point %= (h-1);
		if(point == 0){
			win.clrNumsLines(win.output, 1, h-1);
			win.createOutput();
			win.refreshMyWindow(win.output);
			point = 1;
		}
		usleep(100000);
	}
}
static void* run_input(void *arg)
{
	udpClient *_cli = (udpClient*)arg;
	for(;;){
    	win.createInput();
    	std::string tips = "Please Enter: ";
    	int h, w;
    	getmaxyx(win.input, h, w);
    	win.putStringToWin(win.input, 1, 1, tips);
    	win.refreshMyWindow(win.input);

    	std::string str;
    	win.getStringFromWin(win.input, str);
		sendData.nick_name=nick_name;
		sendData.school=school;
		sendData.msg=str;
		sendData.cmd="None";

		std::string sendString;
		sendData.valueToString(sendString);

    	_cli->sendData(sendString);
    	win.clrNumsLines(win.input, 1, 1);
		usleep(200000);
	}
}

void sendQuit(int sig)
{
	sendData.nick_name=nick_name;
	sendData.school=school;
	sendData.msg="None";
	sendData.cmd="QUIT";
	std::string sendString;
	sendData.valueToString(sendString);
    sig_cli->sendData(sendString);
	exit(0);
}

//./client 126.1.1.1 8080 nick_name school
int main(int argc, char *argv[])
{
	if(argc != 5){
		usage(argv[0]);
		exit(1);
	}
	int port = atoi(argv[2]);
	udpClient cli(argv[1], port);

	sig_cli = &cli;

	struct sigaction act;
	act.sa_handler = sendQuit;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(2, &act, NULL);

	nick_name=argv[3];
	school=argv[4];

	cli.initClient();
	pthread_t top;
	pthread_t middle;
	pthread_t buttom;

	pthread_create(&top, NULL, run_header, NULL);
	pthread_create(&middle, NULL, run_output_friend, (void*)&cli);
	pthread_create(&buttom, NULL, run_input, (void*)&cli);

	pthread_join(top, NULL);
	pthread_join(middle, NULL);
	pthread_join(buttom, NULL);

	return 0;
}






