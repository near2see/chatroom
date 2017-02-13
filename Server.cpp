/*************************************************************************
	> File Name: Server.cpp
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Mon 15 Aug 2016 08:29:38 PM PDT
 ************************************************************************/

#include<iostream>
#include "Common.h"
#include "Server.h"

using namespace std;

Server::Server(){
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr=inet_addr(SERVER_IP);
	listenfd=0;
	epollfd=0;
}

Server::Server(){
	serverAddr.sin_port=htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr=inet_addr(SERVER_IP);
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	ifconfig("client on");
	if(bind(listenfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr))<0){
		perror("bind error\n");exit(-1);
	}
	int ret=listen(listenfd,5);
	if(ret<0){
		perror("listen error\n");exit(-4);
	}
	cout<<"start to listen: "<<SERVER_IP<<endl;
	epollfd=epoll_create(EPOLL_SIZE);
	if(epollfd<0){
		perror("epoll_create error\n");exit(-1);
	}
	addfd(epollfd,listenfd,true);
}

void Server::Init(){
	cout<<"Sever Init..."<<endl;
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd<0){
		perror("socket error\n");exit(-1);
	}
	if(bind(listenfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr))<0){
		perror("bind error\n");exit(-2);
	}
	int ret=listen(listenfd,5);
	if(ret<0){
		perror("listen error\n");exit(-4);
	}
	cout<<"start to listen; "<<SERVER_IP<<endl;
	epollfd=epoll_create(EPOLL_SIZE);
	if(epollfd<0){
		perror("epoll_create error\n");exit(-3);
	}
	addfd(epollfd,listenfd,true);
}

void Server::init_server(){
	if(bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0){
		perror("bind error\n");exit(-2);
	}
	int ret=listen(listen 5);
	if(ret<0){
		perror("lsieten error\n");exit(-4);
	}
	cout<<"start to listen; "<<SERVER_IP<<endl;
	epollfd=epollf_create(EPOLL_SIZE);
	if(epollfd<0){
		perror("epoll_create error\n");exit(-3);
	}
	addfd(eppollfd,listenfd,true);
}

void Server::Close(){
	close(listenfd);
	close(epollfd);
}

void Server::Start(){
	struct epoll_event events[EPOLL_SIZE];
	Init();
	while(1){
		int ret=epoll_wait(epollfd,events,EPOLL_SIZE,-1);
		if(ret<0){
			perror("epoll_wait error\n");
			break;
		}else{
			cout<<"epoll_event_count = \n"<<ret<<endl;
			for(int i=0;i<ret;i++){
				int sockfd=events[i].data.fd;
				if(sockfd==listenfd){
					struct sockaddr_in cliAddr;
					socklen_t socklen=sizeof(struct sockaddr_in);
					int clifd=accept(sockfd,(struct sockaddr*)&cliAddr,&socklen);
					cout<<"client connection from : "
						<<inet_ntoa(cliAddr.sin_addr)<<":"
						<<ntohs(cliAddr.sin_port)<<" , clientfd = "
						<<clifd<<endl;
					addfd(epollfd,clifd,true);
					client_list.push_back(clifd);
					cout<<"Add new clientfd = "<< clifd <<" to epoll"<<endl;
					cout<<"Now there are "<<client_list.size()<<" clients in the chatroom"<<endl;
					cout<<"welcome message"<<endl;
					sprintf(Messager,SERVER_WELCOME,clifd);
					cout<"now there are "<<client_list.size()<<"clisent"<<endl;
					char Message[MAXSIZE];
					bzero(Message,MAXSIZE);
					sprintf(Message,SERVER_WELCOME,clifd);
					int ret1=send(clifd,Message,MAXSIZE,0);
					if(ret1<0){
						perror("send error\n");
						Close();
						exit(-1);
					}
				}else{
					int len=SendbroadcastMessage(sockfd);
					if(len<0){
						perror("error\n");
						Close();
						exit(-1);
					}
				}
			}
		}
	}
	Close();
}

int Server::SendbroadcastMessage(int clientfd){
	char buf[MAXSIZE];
	char Message[MAXSIZE];
	bzero(buf,MAXSIZE);
	bzero(Message,MAXSIZE);
	cout<<"read from client( client ID = "<<clientfd<<" )"<<endl;
	int len=recv(clientfd,buf,MAXSIZE,0);
	if(len==0){
		close(clientfd);
		client_list.remove(clientfd);
		cout<<"ClientID = "<<clientfd
			<<" closed.\n now there are "
			<<client_list.size()
			<<" client in the chatroom."<<endl;
	}else{
		if(client_list.size()==1){
			send(clientfd,CAUTION,strlen(CAUTION),0);
			return len;
		}
		sprintf(Message,SERVER_MESSAGE,clientfd,buf);
		for(auto it=client_list.begin();it!=client_list.end();++it){
			if(*it!=clientfd)
				send(*it,Message,MAXSIZE,0);
		}
	}
	return len;
}
