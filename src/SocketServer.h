#ifndef _SOCKER_SERVER_H
#define _SOCKER_SERVER_H

#include "common.h"

class SocketServer
{
public:
	SocketServer(void);
	~SocketServer(void);
	
public:
	int StartServer(int port = 5001,int threadNum = 10);
	void StopServer();	
	
	static void S_WorkService(void* arg);
	void WorkService(int epollfd);
private:
	int make_socket_nonblock(int sock);	
	int make_socket_reuseable(int sock);
	int make_socket_timeout(int sock,int time);	
	int make_socket_buffsize(int sock,int size);	
	int add_socket_epoll(int epollfd ,int socket,bool oneshot);
	int reset_socket_epoll(int epollfd ,int socket);
	int del_socket_epoll(int epollfd ,int socket);

private:
	bool             m_bstop;                 //是否关闭
	int            	 m_nport;				  //端口号
	int            	 m_epollfd;               //主线程监听epoll 句柄
	int           	 m_bindsocket;            //监听的socket
	int           	 m_nThreadNum;            //接收数据工作线程的数量
	std::vector<int> m_EpollVec ;             //工作线程的epoll句柄集
};

typedef singleton<SocketServer> SOCKETServer;

#endif