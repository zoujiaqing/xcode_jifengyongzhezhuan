#ifndef __SOCKET_WIN32_H__
#define __SOCKET_WIN32_H__
#pragma once

#include <Ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <errno.h>

//socket api from android platform to win32 platform

#define F_GETFL			0
#define F_SETFL			FIONBIO
#define O_NDELAY		1
#define O_NONBLOCK		1
#define MSG_DONTWAIT	0

typedef int socklen_t;
typedef unsigned long in_addr_t;

inline int close(int socket_id)
{
	return closesocket(socket_id);
}

inline int read(int socket_id, char *buff, int len)
{
	return recv(socket_id, buff, len, 0);
}

inline int write(int socket_id, const char *buff, int len)
{
	return send(socket_id, buff, len, 0);
}

inline int fcntl(int fd, int cmd, int flag)
{
	if(cmd == F_GETFL)
	{
		return 0;
	}
	else
	{
		//by benyang:need test
		int res = ioctlsocket(fd, cmd, (u_long *)&flag);
		if(res == SOCKET_ERROR)
		{
			int err = WSAGetLastError();
		}
		return res;
	}
}

inline int ioctl(int socket_id, int flag, unsigned long *value)
{
	if(ioctlsocket(socket_id, flag, value) == 0)
		return 0;
	else
		return -1;
}

inline int inet_aton(const char *listenerAddr, struct in_addr *addr)
{
	addr = (struct in_addr *)inet_addr(listenerAddr);

	if(INADDR_NONE == (unsigned int)addr)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

#endif // __SOCKET_WIN32_H__