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
	
private:
	int            m_nport;					//端口号
	
};

typedef singleton<SocketServer> SOCKETServer;

#endif