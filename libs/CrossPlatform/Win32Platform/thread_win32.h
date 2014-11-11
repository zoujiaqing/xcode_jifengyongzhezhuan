#ifndef __THREAD_WIN32_H__
#define __THREAD_WIN32_H__
#pragma once

#include <windows.h>

#define pthread_t HANDLE
#define pthread_attr_t int

#define PTHREAD_SCOPE_PROCESS	0
#define PTHREAD_SCOPE_SYSTEM	1

//inline int pthread_create(pthread_t *tid, pthread_attr_t *attr, DWORD (*start_rtn)(LPVOID), void *arg)
//{
//	*tid = CreateThread(NULL, NULL, start_rtn, arg, NULL, NULL);
//	if(*tid != NULL)
//	{
//		return 0;
//	}
//	else
//	{
//		return -1;
//	}
//}

inline void pthread_exit(void *reval)
{
	ExitThread(*(int*)reval);
}

inline int pthread_join(pthread_t tid, void **reval)
{
	return WaitForSingleObject(tid, 0);
}

inline int pthread_detach(pthread_t tid)
{
	return 0;
}

inline pthread_t pthread_self()
{
	return GetCurrentThread();
}

inline void usleep(int time)
{
	Sleep(time);
}

inline void pthread_attr_init(pthread_attr_t *attr)
{
	return;
}

inline void pthread_attr_destroy(pthread_attr_t *attr)
{
	return;
}

inline void pthread_attr_setscope(pthread_attr_t *attr, int flag)
{
	return;
}

inline int pthread_attr_getscope(pthread_attr_t *attr)
{
	return PTHREAD_SCOPE_SYSTEM;
}

#endif // __THREAD_WIN32_H__