#include "SocketServer.h"


SocketServer::SocketServer(void) :
 m_nport(5001)
{
}

SocketServer::~SocketServer(void)
{

}

int SocketServer::StartServer(int port)
{
	m_nport = port;
	
	struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, "0.0.0.0", &address.sin_addr );
    address.sin_port = htons( m_nport );
	
	int sock = socket( PF_INET, SOCK_STREAM, 0 );
	if(sock < 0)
	{
		LOG_ERROR("socket error ,errmsg = %s",strerror(errno));
		return SERVER_ERROR;
	}
		
	int reuse = 1;
    if(0 != setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse )))
	{
		LOG_ERROR("setsockopt SO_REUSEADDR error ,errmsg = %s",strerror(errno));
		return SERVER_ERROR;
	}
	int recvbuf = 0;
	int len = sizeof( recvbuf );
	getsockopt( sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, ( socklen_t* )&len);
    LOG_INFO( "the receive buffer size before settting is %d\n", recvbuf );
	
	recvbuf = SOCKET_BUF_SIZE;
    if(0 != setsockopt( sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, sizeof( recvbuf) ))
	{
		LOG_ERROR("setsockopt SO_RCVBUF error ,errmsg = %s",strerror(errno));
		return SERVER_ERROR;
	}
	getsockopt( sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, ( socklen_t* )&len);
    LOG_INFO( "the receive buffer size after settting is %d\n", recvbuf );
	
	int sendbuf = 0;
	getsockopt( sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, ( socklen_t* )&len);
    LOG_INFO( "the tcp send buffer size before setting is %d\n", sendbuf );
	
	sendbuf = SOCKET_BUF_SIZE;
    if(0 != setsockopt( sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, sizeof( sendbuf) ))
	{
		LOG_ERROR("setsockopt SO_SNDBUF error ,errmsg = %s",strerror(errno));
		return SERVER_ERROR;
	}

	getsockopt( sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, ( socklen_t* )&len);
    LOG_INFO( "the tcp send buffer size after setting is %d\n", sendbuf );
	
	
	return SERVER_OK;
}

void SocketServer::StopServer()
{

}