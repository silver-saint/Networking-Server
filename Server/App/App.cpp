#include "App.h"

void Server::Run()
{
	try
	{
		server.Init();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void Client::Run()
{
	try
	{
		client.Init();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
