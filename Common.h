/*************************************************************************
	> File Name: Common.h
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Mon 15 Aug 2016 08:03:18 PM PDT
 ************************************************************************/
#ifndef CHATROOM_COMMON_H
#define CHATROOM_COMMON_H

#include<iostream>
#include<list>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<errno.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

using namespace std;

#define SERVER_IP "127.0.0.1"

#define SERVER_PORT 8888

#define EPOLL_SIZE 5000

#define MAXSIZE 0xFFFF

#define SERVER_WELCOME "Welcome you join to the chat room! Your char ID is: Client #%d"

#define SERVER_MESSAGE "ClientID %d say >> %s"

#define EXIT "EXIT"

#define CAUTION "There is only one in the char room !"

static void addfd(int epollfd,int fd,bool enable_et){
	struct epoll_event ev;
	ev.data.fd=fd;
	ev.events=(enable_et)? EPOLLIN|EPOLLET:EPOLLIN;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
	fcntl(fd,F_SETFD,fcntl(fd,F_GETFD,0)|O_NONBLOCK);
	printf("fd add to epoll\n\n");
}

#endif
