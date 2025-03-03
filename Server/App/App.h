#pragma once
#include "../TCPServer/TCPServer.h"
struct App
{
public:
	void Run();
private:
	Net::TCPServer server = { "127.0.0.1", 8080 };
};

