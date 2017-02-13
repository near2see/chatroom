/*************************************************************************
	> File Name: Client.h
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Mon 15 Aug 2016 10:51:54 PM PDT
 ************************************************************************/
#ifndef CHATROOM_CLIENT_H
#define CHATROOM_CLIENT_H

#include<string>
#include"Common.h"
using namespace std;

class Client{
public:
	Client();
	void Connect();
	void Close();
	void Start();
private:
	int sock;
	int pid;
	int epollfd;
	int pipe_fd[2];
	bool isClientwork;
	char message[MAXSIZE];
	struct sockaddr_in serverAddr;
};

#endif
