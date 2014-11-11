#ifndef LOG_H
#define LOG_H
#ifdef FRONT
#define DEBUG_LOG(fmt, ...) 
#define TRACE_LOG(fmt, ...) 
#define KDEBUG_LOG(fmt, ...) 
#define INFO_LOG(fmt, ...) 
#define KINFO_LOG(fmt, ...) 
#define ERROR_LOG(fmt, ...)
#define KERROR_LOG(fmt, ...)
#define ERROR_RETURN_VOID(fmt, ...)
#define ERROR_RETURN(fmt, ...)
#else
#include <libtaomee/log.h>
#endif

#endif
