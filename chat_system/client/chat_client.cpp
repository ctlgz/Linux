#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include "udp_client.h"
#include "data.h"
#include "window.h"


static void usage(const char *proc)
{
	std::cout << "\nUsage:\n\t" << proc << " [server_ip] [server_port]\n" << std::endl;
}

std::string nick_name;
std::string school;
udp_client *cp = NULL;
window *wp = NULL;
std::vector<std::string> friends;

void addUser(const std::string &f)
{
	std::vector<std::string>::iterator iter = friends.begin();
	for(; iter != friends.end(); iter++){
		if(*iter == f){
			return;
		}
	}
	friends.push_back(f);
}

void delUser(const std::string &f)
{
	std::vector<std::string>::iterator iter = friends.begin();
	for(; iter != friends.end();){
		if( *iter == f ){
			iter = friends.erase(iter);
		}else{
			iter++;
		}
	}
}

static void *run_header(void *arg)
{
	window *wp = (window*)arg;
	std::string tips = "Welcome to Chat System!";
	int i = 1;
	int y, x;
	int dir = 0;
	while(1){
		wp->drawHeader();
		getmaxyx(wp->header, y, x);
	    wp->putStrToWin(wp->header, y/2, i, tips);
		wrefresh(wp->header);
		usleep(200000);
		if(dir == 0 ){
			i++;
			if(i >= x - tips.size()){
				dir = 1;
			}
		}
		if( dir == 1 ){
			i--;
			if(i <= 1){
				dir = 0;
			}
		}
	}
	return (void *)0;
}

static void *run_output_flist(void *arg)
{
	window *wp = (window*)arg;

	wp->drawOutput();
	wrefresh(wp->output);
	wp->drawFlist();
	wrefresh(wp->flist);

	data d;
	std::string jsonString;
	std::string outputString;
	std::string f;
	int i = 1;
	int j = 1;
	int y, x;
	while(1){
		getmaxyx(wp->output, y, x);

		cp->recvData(jsonString);
		d.dataUnserialize(jsonString);

		outputString = d.nick_name;
		outputString += "-";
		outputString += d.school;
		f = outputString;

		if(d.cmd == "QUIT"){
	    	delUser(f);
		}else{
	    	outputString += "# ";
	    	outputString += d.msg;
            wp->putStrToWin(wp->output, i, 2, outputString);
	    	wrefresh(wp->output);
	    	if(++i > y - 2){
	    		wp->drawOutput();
	    		i = 1;
	    	}

	    	addUser(f);
		}
		wp->drawFlist();
		for(j = 0; j < friends.size(); j ++){
			wp->putStrToWin(wp->flist, j + 1, 2, friends[j]);
		}
		wrefresh(wp->flist);
	}
}

static void *run_input(void *arg)
{
	window *wp = (window*)arg;
	std::string msg;
	std::string tips = "Please Enter# ";
	while(1){
		data d;
		d.nick_name = nick_name;
		d.school = school;
		d.cmd = "";
        wp->drawInput();
        wp->putStrToWin(wp->input, 1, 2, tips);
        wrefresh(wp->input);
        wp->getStrFromWin(wp->input, d.msg);

		std::string sendString;
		d.dataSerialize(sendString);

		cp->sendData(sendString);

	}
	return (void *)0;
}

void sendQuit(int sig)
{
	std::string quitString;
	data d;
	d.nick_name = nick_name;
	d.school = school;
	d.msg = "";
	d.cmd = "QUIT";
	d.dataSerialize(quitString);
	cp->sendData(quitString);
	delete wp;
	exit(0);
}

int main(int argc, char *argv[])
{
	if(argc != 3){
		usage(argv[0]);
		return 1;
	}
	std::cout << "Please Enter Nick_Name: ";
	std::cin >> nick_name;
	std::cout << "Please Enter School: ";
	std::cin >> school;

	udp_client client(argv[1], atoi(argv[2]));
	client.initClient();
	cp = &client;

	wp = new window();
	signal(2, sendQuit);

	pthread_t header, output_flist, input;
	pthread_create(&header, NULL, run_header, wp);
	pthread_create(&output_flist, NULL, run_output_flist, wp);
	pthread_create(&input, NULL, run_input, wp);

	pthread_join(header, NULL);
	pthread_join(output_flist, NULL);
	pthread_join(input, NULL);
	return 0;
}














