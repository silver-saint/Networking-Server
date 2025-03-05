#pragma once
#include "../TCPServer/TCPServer.h"
#include "../TCPClient/TCPClient.h"
struct Server
{
	void Run();
private:
	Net::TCPServer server = { "127.0.0.1", 8080 };
};
struct Client
{
	void Run();
private:
	Net::TCPClient client = { "127.0.0.1", 8080 };
};
