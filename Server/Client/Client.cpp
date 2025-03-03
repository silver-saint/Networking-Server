#include "Client.h"

Net::Client::Client(const std::string& address, i16 port) noexcept
	: m_socket {0}, 
	  m_ip {address},
	  m_port{port} {}

Net::Client::Client(Client&& other) noexcept
	: m_socket{std::move(other.m_socket)},
	  m_ip{std::move(other.m_ip)},
	  m_port{std::move(other.m_port)}
{}

void Net::Client::Init()
{
	return;
}

i32 Net::Client::Connect()
{
	return 0;
}

i32 Net::Client::Send(const std::string& buffer, i32 len)
{
	return 0;
}

void Net::Client::ReceiveLoop(Client* client)
{
	return;
}

void Net::Client::Close()
{
	return;
}
