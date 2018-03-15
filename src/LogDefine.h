#ifndef  _DCLOGDEFINE_H
#define  _DCLOGDEFINE_H

#include "xlogger.h"


#define DEFAULT "default"

#define LOG_DEBUG(args...)  log4cxx_package(LOG_DEBUG_,DEFAULT, __FILE__, __FUNCTION__,__LINE__, ##args)
#define LOG_INFO(args...)   log4cxx_package(LOG_INFO_, DEFAULT, __FILE__, __FUNCTION__,__LINE__, ##args)
#define LOG_WARN(args...)   log4cxx_package(LOG_WARN_, DEFAULT, __FILE__, __FUNCTION__,__LINE__,##args)
#define LOG_ERROR(args...)  log4cxx_package(LOG_ERROR_,DEFAULT, __FILE__, __FUNCTION__,__LINE__, ##args)


#define SERVER_MSG "server"

#define DC_DEBUG(args...)  log4cxx_package(LOG_DEBUG_,SERVER_MSG, __FILE__, __FUNCTION__,__LINE__, ##args)
#define DC_INFO(args...)   log4cxx_package(LOG_INFO_, SERVER_MSG, __FILE__, __FUNCTION__,__LINE__, ##args)
#define DC_WARN(args...)   log4cxx_package(LOG_WARN_, SERVER_MSG, __FILE__, __FUNCTION__,__LINE__,##args)
#define DC_ERROR(args...)  log4cxx_package(LOG_ERROR_,SERVER_MSG, __FILE__, __FUNCTION__,__LINE__, ##args)

#endif
