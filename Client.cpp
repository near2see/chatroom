/*************************************************************************
	> File Name: Client.cpp
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Mon 15 Aug 2016 11:02:16 PM PDT
 ************************************************************************/

#include<iostream>
#include"Common.h"
#include"Client.h"

using namespace std;

Client::Client(){
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_addr.s_addr=inet_addr(SERVER_IP);
	serverAddr.sin_port=htons(SERVER_PORT);
	sock=0;
	pid=0;
	epollfd=0;
	isClientwork=true;
}

void Client::Connect(){
	cout<<"connect Server IP "<<SERVER_IP<<":"<<SERVER_PORT<<endl;
	sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0){
		perror("socket error\n");
		exit(-1);
	}
	int ret=connect(sock,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
	if(ret<0){
		perror("connect error\n");
		exit(-1);
	}
	if((pipe(pipe_fd))<0){
		perror("pipe error\n");
		exit(-1);
	}
	epollfd=epoll_create(EPOLL_SIZE);
	if(epollfd<0){
		perror("epoll_create error\n");
		exit(-1);
	}
	addfd(epollfd,sock,true);
	addfd(epollfd,pipe_fd[0],true);
}

void Client::Close(){
	if(pid){
		close(pipe_fd[0]);
		close(sock);
	}else{
		close(pipe_fd[1]);
	}
}

void Client::Start(){
	struct epoll_event events[2];
	Connect();
	if((pid=fork())<0){
		perror("fork error\n");close(sock);exit(-1);
	}else if(pid==0){
		close(pipe_fd[0]);
		cout<<"Please input 'exit' to exit the chatroom"<<endl;
		while(isClientwork){
			bzero(message,MAXSIZE);
			fgets(message,MAXSIZE,stdin);
			if(strncasecmp(message,EXIT,strlen(EXIT))==0){
				isClientwork=false;
			}else{
				int nwrite=write(pipe_fd[1],message,strlen(message)-1);
				if(nwrite==0){
					perror("write error\n");
					exit(-1);
				}
			}
		}
	}else{
		close(pipe_fd[1]);
		while(isClientwork){
			int num=epoll_wait(epollfd,events,2,-1);
			for(int i=0;i<num;i++){
				bzero(message,MAXSIZE);
				int fd=events[i].data.fd;
				if(fd==sock){
					int ret=recv(fd,message,MAXSIZE,0);
					if(ret==0){
						cout<<"Server closed connection: "<<sock<<endl;
						close(sock);isClientwork=false;
					}else{
						printf("%s\n",message);
					}
				}else{
					int nr=read(fd,message,MAXSIZE);
					if(nr==0){
						isClientwork=false;
					}else{
						send(sock,message,MAXSIZE,0);
					}
				}
			}
		}
	}
	Close();
}

