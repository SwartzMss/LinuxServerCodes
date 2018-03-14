#include "SocketServer.h"


int main(int argc,char**argv)
{
	LOG_INFO("application is starting !!!");
	if(SERVER_OK != SOCKETServer::Instance()->StartServer(5001))
	{
		LOG_ERROR("StartServer error ,application will stop !!!");
		SOCKETServer::Instance()->StopServer();
		return -1;
	}
	LOG_INFO("application start success !!!");
	
	return 0;
}