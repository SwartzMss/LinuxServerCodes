#ifndef  _XLOGGER_H
#define  _XLOGGER_H

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif


typedef enum _LOG_LEVEL
{
	LOG_TRACE_ = 0,
	LOG_DEBUG_,
	LOG_INFO_,
	LOG_WARN_,
	LOG_ERROR_,
	LOG_FATAL_
}LOG_LEVEL;


void log_init();

void log4cxx_package(IN const LOG_LEVEL level, IN const char* user, IN const char* file, IN const char* function, IN const int line, IN const char* format, ...);


#endif

