#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <array>
#include "../Types/types.h"
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
using socket_t = SOCKET;
#else
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using socket_t = int;
#endif
#endif
namespace Net
{
	inline void close(socket_t s)
	{
		#ifdef _WIN32	
		closesocket(s);
		#else
		close(s);
		#endif
	}
}