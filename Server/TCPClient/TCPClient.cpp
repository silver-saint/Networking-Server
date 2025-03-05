#include "TCPClient.h"

Net::TCPClient::TCPClient(const std::string& address, i16 port) noexcept
	: m_serverSocket{INVALID_SOCKET},
	  m_serverAddress{0},
	  m_ip {address},
	  m_port{port} {}

Net::TCPClient::TCPClient(TCPClient&& other) noexcept
	: m_serverSocket{std::move(other.m_serverSocket)},
	  m_ip{std::move(other.m_ip)},
	  m_port{std::move(other.m_port)}
{}

void Net::TCPClient::Init()
{
	try
	{
		InitWinSocket();
		CreateSocket();
		BindAddress();
		Connect();
		Receive();
		Send();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

i32 Net::TCPClient::CreateSocket()
{
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET)
	{
		std::cout << "Error code: " << WSAGetLastError() << '\n';
		throw std::runtime_error("Socket creation failed");
	}
	return 1;
}
i32 Net::TCPClient::InitWinSocket()
{
	WSADATA wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (Result != 0)
	{
		std::cout << "WSAStartup failed: " << Result << '\n';
		throw std::runtime_error("Failed to init WinSockets");
	}
	std::cout << "Winsock initialized.\n";
	std::wcout << "Current version: " << LOBYTE(wsaData.wVersion) << "." << HIBYTE(wsaData.wVersion) << std::endl;
	return 1;
}
i32 Net::TCPClient::Receive()
{
	std::vector<char> buffer(256);
	i32 bytesReceived = recv(m_serverSocket, buffer.data(), buffer.size(), 0);
	if (bytesReceived <= 0) {
		throw std::runtime_error("Couldn't receive buffer\n");
	}
	buffer.resize(bytesReceived);
	std::cout << "Received from Server: " << std::string(buffer.begin(), buffer.end()) << '\n';
	return 1;
}

i32 Net::TCPClient::Send()
{
	const std::string res = "Hello from Client";
	i32 sendRes = send(m_serverSocket, res.data(), res.length(), 0);
	if (sendRes == SOCKET_ERROR)
	{
		throw std::runtime_error("Failed to send response!\n");
	}

	return 1;
}

i32 Net::TCPClient::Connect()
{
	std::cout << "Connecting to " << m_ip << ":" << m_port << "..." << std::endl;
	i32 result = connect(m_serverSocket, reinterpret_cast<sockaddr*>(&m_serverAddress), sizeof(m_serverAddress));
	if (result == SOCKET_ERROR)
	{
		throw std::runtime_error("Couldn't connect to server\n");
	}
	return 1;
}

i32 Net::TCPClient::BindAddress()
{
	m_serverAddress.sin_family = AF_INET;
	m_serverAddress.sin_port = htons(m_port);
	i32 bindAddressResult = inet_pton(AF_INET, m_ip.c_str(), &m_serverAddress.sin_addr);
	if (bindAddressResult == SOCKET_ERROR)
	{
		throw std::runtime_error("binding inet_pton Failed\n");
	}
	return 1;
}


