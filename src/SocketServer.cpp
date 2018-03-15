#include "SocketServer.h"


SocketServer::SocketServer(void) :
  m_nport(5001)
 ,m_epollfd(-1)
 ,m_bindsocket(-1)
 ,m_bstop(false)
{
}

SocketServer::~SocketServer(void)
{

}

void SocketServer::S_WorkService(void* arg)
{
	SocketServer* pServer = (SocketServer*)arg;
	pServer->WorkService();
}

void SocketServer::WorkService()
{
	struct sockaddr_in client_address;
	socklen_t client_addrlength = sizeof( client_address );
	int sockfd = accept( m_bindsocket, ( struct sockaddr* )&client_address, &client_addrlength );
	if(sockfd < 0)
	{
		DC_ERROR("accept error ,errmsg = %s",strerror(errno));
		return ;
	}
	
	char remoteAddress[INET_ADDRSTRLEN ] = {0};
	inet_ntop( AF_INET, &client_address.sin_addr, remoteAddress, INET_ADDRSTRLEN );
	int remotePort = ntohs( client_address.sin_port );
	DC_INFO("%s:%d connect" , remoteAddress, remotePort );
	
	epoll_event events[ MAX_EVENT_NUMBER ];
    int epollfd = epoll_create( 5 );
	
	if(0 != add_socket_epoll(epollfd, sockfd,true))
	{
		goto _exit;
	}
	
	/*设置非阻塞*/
	if(0 != make_socket_nonblock(sockfd))
	{
		goto _exit;
	}
	
	 while( !m_bstop )
    {
        int event_num = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );
		if ( event_num < 0 )
        {
			DC_ERROR("epoll_wait error ,errmsg = %s",strerror(errno));
            break;
        }
		
		for ( int i = 0; i < event_num; i++ )
        {
            int sock = events[i].data.fd;
            if ( events[i].events & EPOLLIN &&(sock == sockfd))
            {
				char* pRecvBuff = new  char[SOCKET_BUF_SIZE];
				int nRemainDataSize = 0;
				 while( true )
				 {
					int nBytesThisTime = recv( sock, pRecvBuff + nRemainDataSize, SOCKET_BUF_SIZE -1-nRemainDataSize, 0 );
					if( nBytesThisTime < 0 )
					{
						if( ( errno == EAGAIN ) || ( errno == EWOULDBLOCK ) )
						{
							 break;
						}
						DC_ERROR("socket errormsg = %s , %s:%d close",strerror(errno),remoteAddress ,remotePort);
						goto _exit;
					}
					else if (nBytesThisTime == 0)
					{
						DC_ERROR("socket errormsg = %s , %s:%d close",strerror(errno),remoteAddress ,remotePort);
						goto _exit;
					}
					nRemainDataSize += nBytesThisTime;
				 }
				
				DC_INFO("recv data size = %d" ,nRemainDataSize);
				
				if(0 != reset_socket_epoll(epollfd, sockfd))
				{
					goto _exit;
				}
            }
            else if( events[i].events & ( EPOLLRDHUP | EPOLLHUP | EPOLLERR ) )
            {
               DC_ERROR("socket errormsg = %s , %s:%d close",strerror(errno),remoteAddress ,remotePort);
			   goto _exit;
            }
			else
			{
				DC_ERROR("socket errormsg = %s , %s:%d close",strerror(errno),remoteAddress ,remotePort);
			}
        }
		
	}
	
_exit:
	del_socket_epoll(epollfd,sockfd);
	close(sockfd);
}
int SocketServer::StartServer(int port)
{
	m_nport = port;
	
	m_bindsocket = socket( PF_INET, SOCK_STREAM, 0 );
	if(m_bindsocket < 0)
	{
		DC_ERROR("socket error ,errmsg = %s",strerror(errno));
		return SERVER_ERROR;
	}
	
	/*地址可复用 time_wait*/	
	if(0 != make_socket_reuseable(m_bindsocket))
	{
		return SERVER_ERROR;
	}
	
	/*设置超时时间*/
	if(0 != make_socket_timeout(m_bindsocket,SOCKET_TIMEOUT))
	{
		return SERVER_ERROR;
	}
	
	/*设置缓冲区大小*/
	if(0 != make_socket_buffsize(m_bindsocket,SOCKET_BUF_SIZE))
	{
		return SERVER_ERROR;
	}
	
	/*设置非阻塞*/
	if(0 != make_socket_nonblock(m_bindsocket))
	{
		return SERVER_ERROR;
	}
	
	/*绑定地址和端口*/
	struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, "0.0.0.0", &address.sin_addr );
    address.sin_port = htons( m_nport );
	if(0 != bind( m_bindsocket, ( struct sockaddr* )&address, sizeof(address) ))
	{
		DC_ERROR("bind %d error ,errmsg = %s",m_nport,strerror(errno));
		return SERVER_ERROR;
	}
	
	/*监听端口*/
	if(0 != listen(m_bindsocket, 128))
	{
		DC_ERROR("bind %d error ,errmsg = %s",m_nport,strerror(errno));
		return SERVER_ERROR;
	}
	
	/*epoll 监听*/
    epoll_event events[ MAX_EVENT_NUMBER ];
    m_epollfd = epoll_create( 5 );
	if(m_epollfd == -1)
	{
		DC_ERROR("epoll_create  error ,errmsg = %s",strerror(errno));
		return SERVER_ERROR;
	}
	if(0 != add_socket_epoll(m_epollfd, m_bindsocket,false))
	{
		return SERVER_ERROR;
	}
	
    while( 1 )
    {
        int event_num = epoll_wait( m_epollfd, events, MAX_EVENT_NUMBER, -1 );
        if ( event_num < 0 && (errno != EINTR))
        {
			DC_ERROR("epoll_wait error ,errmsg = %s",strerror(errno));
            break;
        }
    
        for ( int i = 0; i < event_num; i++ )
        {
            int sockfd = events[i].data.fd;
            if ( sockfd == m_bindsocket )
            {
				swartz_thread_detached_create((void*)S_WorkService, this, 0, 0);
            }
            else
            {
                DC_INFO("other thing happened ,event = %d ",events[i].events);
            } 
        }
    }
	
	m_bstop = true;
	StopServer();
	
	return SERVER_OK;
}



int SocketServer::make_socket_nonblock(int sock)
{
	int flags;
	if ((flags = fcntl(sock, F_GETFL, NULL)) < 0) 
	{
		DC_ERROR("fcntl(%d, F_GETFL) ,ermsg = %s", sock,strerror(errno));
		return SERVER_ERROR;
	}
	if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		DC_ERROR("fcntl(%d, F_SETFL) O_NONBLOCK ,ermsg = %s", sock,strerror(errno));
		return SERVER_ERROR;
	}
	return SERVER_OK;
	
}

int SocketServer::make_socket_reuseable(int sock)
{
	int reuse = 1;
    if(0 != setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse )))
	{
		DC_ERROR("setsockopt SO_REUSEADDR error ,errmsg = %s",strerror(errno));
		return SERVER_ERROR;
	}
	struct linger tcpLinger;
	tcpLinger.l_onoff  = 1;
	tcpLinger.l_linger = 0;

	if (0 != setsockopt(sock, SOL_SOCKET, SO_LINGER, &tcpLinger, sizeof(tcpLinger)))
	{
		DC_ERROR("setsockopt SO_LINGER error ,errmsg = %s",strerror(errno));
		return SERVER_ERROR;
	}
	return SERVER_OK;	
}

int SocketServer::make_socket_timeout(int sock,int time)
{
	/*查询和设置发送超时时间*/
    struct timeval send_timeout;  
	send_timeout.tv_sec = time;  
    send_timeout.tv_usec = 0; 
	int len = sizeof( timeval );	
    if(0 != setsockopt( sock, SOL_SOCKET, SO_SNDTIMEO, &send_timeout, sizeof(send_timeout) ))
	{
		DC_ERROR("setsockopt SO_SNDTIMEO error ,errmsg = %s",strerror(errno));
		return SERVER_ERROR;
	}
	getsockopt( sock, SOL_SOCKET, SO_SNDTIMEO, &send_timeout, ( socklen_t* )&len);
    DC_INFO( "the send timeout after settting is %ds", send_timeout.tv_sec/1000 );
	
	/*查询和设置接收超时时间*/
	struct timeval recv_timeout;  
	recv_timeout.tv_sec = time;  
    recv_timeout.tv_usec = 0;  
    if(0 != setsockopt( sock, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, sizeof( recv_timeout) ))
	{
		DC_ERROR("setsockopt SO_RCVTIMEO error ,errmsg = %s",strerror(errno));
		return SERVER_ERROR;
	}
	getsockopt( sock, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, ( socklen_t* )&len);
    DC_INFO( "the recv timeout after setting is %ds", recv_timeout.tv_sec/1000 );
	return SERVER_OK;
}
int SocketServer::make_socket_buffsize(int sock,int size)
{
	/*查询和设置接收缓冲区*/
	int recvbuf = 0;
	int len = sizeof( recvbuf );
	getsockopt( sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, ( socklen_t* )&len);
    DC_INFO( "the receive buffer size before settting is %d", recvbuf );
	
	recvbuf = size;
    if(0 != setsockopt( sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, sizeof( recvbuf) ))
	{
		DC_ERROR("setsockopt SO_RCVBUF error ,errmsg = %s",strerror(errno));
		return SERVER_ERROR;
	}
	getsockopt( sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, ( socklen_t* )&len);
    DC_INFO( "the receive buffer size after settting is %d", recvbuf );
	
	/*查询和设置发送缓冲区*/
	int sendbuf = 0;
	getsockopt( sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, ( socklen_t* )&len);
    DC_INFO( "the tcp send buffer size before setting is %d", sendbuf );
	
	sendbuf = size;
    if(0 != setsockopt( sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, sizeof( sendbuf) ))
	{
		DC_ERROR("setsockopt SO_SNDBUF error ,errmsg = %s",strerror(errno));
		return SERVER_ERROR;
	}
	getsockopt( sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, ( socklen_t* )&len);
    DC_INFO( "the tcp send buffer size after setting is %d", sendbuf );
	return SERVER_OK;
}

int SocketServer::add_socket_epoll(int epollfd ,int socket,bool oneshot)
{
	epoll_event event;
    event.data.fd = socket;
    event.events = EPOLLIN | EPOLLET ;
	if( oneshot )
    {
        event.events |= EPOLLONESHOT;
    }
    if(0 != epoll_ctl( epollfd, EPOLL_CTL_ADD, socket, &event ))
	{
		DC_ERROR("epoll_ctl  error ,errmsg = %s",strerror(errno));
		return SERVER_ERROR;
	}
	return SERVER_OK;
}

int SocketServer::reset_socket_epoll(int epollfd ,int socket)
{
	epoll_event event;
    event.data.fd = socket;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    if(0 != epoll_ctl( epollfd, EPOLL_CTL_MOD, socket, &event ))
	{
		DC_ERROR("epoll_ctl  error ,errmsg = %s",strerror(errno));
		return SERVER_ERROR;
	}
	return SERVER_OK;
}

int SocketServer::del_socket_epoll(int epollfd ,int socket)
{
    if(0 != epoll_ctl( epollfd, EPOLL_CTL_DEL, socket,0))
	{
		DC_ERROR("epoll_ctl  error ,errmsg = %s",strerror(errno));
		return SERVER_ERROR;
	}
	close(epollfd);
	return SERVER_OK;
}

void SocketServer::StopServer()
{
	/*关闭所有的文件描述符*/
	for (int sockfd = 3; sockfd < getdtablesize(); sockfd++)
	{
		close(sockfd);
	}
}