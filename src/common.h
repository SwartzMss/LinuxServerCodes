#ifndef _COMMON_H_
#define _COMMON_H_

/*network*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

/*linux*/
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>

/*stl*/
#include <string>
#include <iostream>
#include <map>
#include <list>
#include <vector>
using namespace std;

/*ansi*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*swartz*/
#include "swartz_mutexEx.h"
#include "swartz_mutex.h"
#include "swartz_singleton.h"
#include "swartz_mempool.h"
#include "swartz_thread.h"

/*xlogger*/
#include "LogDefine.h"


#define SERVER_OK     (0)
#define SERVER_ERROR  (-1)

#define SOCKET_BUF_SIZE (1024*1024)
#define SOCKET_TIMEOUT  (15*1000)

#define MAX_EVENT_NUMBER 10*1024

#endif