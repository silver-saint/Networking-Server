#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "../Types/types.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
// _WIN32
using socket_t = SOCKET;
class TCPServer
{
public:
	TCPServer(const std::string& address, i16 port);
	void Init();
	i32 InitWinSocket();
	i32 InitServerSocket();
	i32 InitClientSocket();
	i32 ConnectToServer();
	i32 InitServerAddress();
	i32 InitListener();
	i32 ReceiveBytes();
	i32 ReceiveResponse();
	~TCPServer();

private:
	socket_t serverSocket;
	socket_t clientSocket;
	sockaddr_in serverAddress;
	sockaddr_in clientAddress;
	std::string ip;
	i16 port;
};
#else
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using socket_t = int; 
#endif
#endif
namespace net
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

