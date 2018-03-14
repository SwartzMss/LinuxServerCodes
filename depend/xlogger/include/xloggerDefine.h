#ifndef  _LOGDEFINE_H
#define  _LOGDEFINE_H

#include "xlogger.h"


#define DEFAULT "default"

#define XLOG_DEBUG(args...)  log4cxx_package(LOG_DEBUG_,DEFAULT, __FILE__, __FUNCTION__,__LINE__, ##args)
#define XLOG_INFO(args...)   log4cxx_package(LOG_INFO_, DEFAULT, __FILE__, __FUNCTION__,__LINE__, ##args)
#define XLOG_WARN(args...)   log4cxx_package(LOG_WARN_, DEFAULT, __FILE__, __FUNCTION__,__LINE__,##args)
#define XLOG_ERROT(args...)  log4cxx_package(LOG_ERROR_,DEFAULT, __FILE__, __FUNCTION__,__LINE__, ##args)
