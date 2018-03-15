#include "SocketServer.h"


int main(int argc,char**argv)
{
	LOG_INFO("application is starting !!!");
	struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    if (sigaction( SIGPIPE, &sa, NULL ) == -1 )
	{
		LOG_ERROR("sigaction SIGPIPE error,errmsg = %s ",strerror(errno));
	}
	if(SERVER_OK != SOCKETServer::Instance()->StartServer(5001))
	{
		LOG_ERROR("StartServer error ,application will stop !!!");
		SOCKETServer::Instance()->StopServer();
		return -1;
	}
	return 0;
}