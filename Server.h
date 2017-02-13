/*************************************************************************
	> File Name: Server.h
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Mon 15 Aug 2016 08:22:29 PM PDT
 ************************************************************************/

#ifndef CHATROOM_SERVER_H
#define CHATROOM_SERVER_H

#include<string>
#include"Common.h"

using namespace std;

class Server{
public:
	Server();
	void Init();
	void Close();
	void Start();
private:
	int SendbroadcastMessage(int clientfd);
	struct sockaddr_in serverAddr;
	int listenfd;
	int epollfd;
	list<int>client_list;
};

#endif
