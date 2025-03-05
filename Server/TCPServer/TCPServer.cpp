#include "TCPServer.h"

Net::TCPServer::TCPServer(const std::string& address, i16 port) noexcept
	: m_serverSocket{INVALID_SOCKET},
	  m_clientSocket{INVALID_SOCKET}, 
	  m_serverAddress{}, 
	  m_clientAddress{}, 
	  m_ip{address}, 
	  m_port{port} {}

Net::TCPServer::TCPServer(TCPServer&& other) noexcept
	: m_serverSocket {std::move(other.m_serverSocket)},
	  m_clientSocket {std::move(other.m_clientSocket)},
	  m_serverAddress {std::move(other.m_serverAddress)},
	  m_clientAddress {std::move(other.m_clientAddress)},
	  m_ip {std::move(other.m_ip)}, 
	  m_port{ std::move(other.m_port) } {}

void Net::TCPServer::Init()
{
	try {
#if _WIN32
		InitWinSocket();
#endif
		CreateSocket();
		BindSocket();
		ListenToPort();
		AcceptConnections();
		Send();
		Receive();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

i32 Net::TCPServer::InitWinSocket()
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

i32 Net::TCPServer::CreateSocket()
{
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET)
	{
		std::cout << "Error code: " << WSAGetLastError() << '\n';
		throw std::runtime_error("Socket creation failed");
	}
	return 1;
}

i32 Net::TCPServer::AcceptConnections()
{
	i32 clientAddrLen = sizeof(m_clientAddress);
	m_clientSocket = accept(m_serverSocket, reinterpret_cast<sockaddr*>(&m_clientAddress), &clientAddrLen);
	if (m_clientSocket == SOCKET_ERROR)
	{
		throw std::runtime_error("Accept Failed\n");
	}
	std::cout << "Client connected.\n";

	return 1;
}

i32 Net::TCPServer::BindSocket()
{
	m_serverAddress.sin_family = AF_INET;
	m_serverAddress.sin_port = htons(m_port);
	i32 bindAddressResult = inet_pton(AF_INET, m_ip.c_str(), &m_serverAddress.sin_addr);
	if (bindAddressResult == SOCKET_ERROR)
	{
		throw std::runtime_error("binding inet_pton Failed\n");
	}
	i32 bindResult = bind(m_serverSocket, reinterpret_cast<sockaddr*>(&m_serverAddress), sizeof(m_serverAddress));
	if (bindResult == SOCKET_ERROR)
	{
		throw std::runtime_error("Bind failed\n");
	}
	return 1;
}

i32 Net::TCPServer::ListenToPort()
{
	int listenResult = listen(m_serverSocket, SOMAXCONN);
	if (listenResult == SOCKET_ERROR)
	{
		throw std::runtime_error("Listening failed\n");
	}
	std::cout <<"Server is listening on port " << m_serverAddress.sin_port << "...\n";

	return 1;

}

i32 Net::TCPServer::Receive()
{
	constexpr size_t bufferSize = 1024;
	std::vector<char> buffer(bufferSize);
	i32 bytesReceived = recv(m_serverSocket, buffer.data(), buffer.size(), 0);
	if (bytesReceived <= 0) {
		throw std::runtime_error("Couldn't receive buffer\n");
	}
	buffer.resize(bytesReceived);
	std::cout << "Received from Client: " << std::string(buffer.begin(), buffer.end()) << '\n';
	return 1;
}

i32 Net::TCPServer::Send()
{
	std::string res = "Hello from Server";
	i32 sendRes = send(m_serverSocket, res.c_str(), res.length(), 0);
	if (sendRes == SOCKET_ERROR)
	{
		throw std::runtime_error("Failed to send response!\n");
	}
	return 1;
}

Net::TCPServer::~TCPServer()
{
	Net::close(m_clientSocket);
	Net::close(m_serverSocket);
	WSACleanup();
}
