#ifndef _SOCKER_SERVER_H
#define _SOCKER_SERVER_H

#include "common.h"

class SocketServer
{
public:
	SocketServer(void);
	~SocketServer(void);
	
public:
	int StartServer(int port = 5001);
	void StopServer();	
	
	static void S_WorkService(void* arg);
	void WorkService();
private:
	int make_socket_nonblock(int sock);	
	int make_socket_reuseable(int sock);
	int make_socket_timeout(int sock,int time);	
	int make_socket_buffsize(int sock,int size);	
	int add_socket_epoll(int epollfd ,int socket,bool oneshot);
	int reset_socket_epoll(int epollfd ,int socket);
	int del_socket_epoll(int epollfd ,int socket);

private:
	bool           m_bstop;                 //是否关闭
	int            m_nport;					//端口号
	int            m_epollfd;               //epoll 句柄
	int            m_bindsocket;            //监听的socket
	
};

typedef singleton<SocketServer> SOCKETServer;

#endif