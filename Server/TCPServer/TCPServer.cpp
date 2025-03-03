#include "TCPServer.h"

Net::TCPServer::TCPServer(const std::string& address, i16 port) noexcept
	: m_serverSocket{0},
	  m_clientSocket{0}, 
	  m_serverAddress{0}, 
	  m_clientAddress{0}, 
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
#if _WIN32
	if (InitWinSocket())
	{

	}
#endif
	InitServerSocket();
	InitServerAddress();
	InitListener();
	InitClientSocket();
	ReceiveBytes();
	ReceiveResponse();
}

i32 Net::TCPServer::InitWinSocket()
{
	WSADATA wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (Result != 0)
	{
		std::cout << "WSAStartup failed: " << Result << '\n';
		return -1;
	}
	std::cout << "Winsock initialized.\n";
	std::wcout << "Current version: " << LOBYTE(wsaData.wVersion) << "." << HIBYTE(wsaData.wVersion) << std::endl;
	return 1;
}

i32 Net::TCPServer::InitServerSocket()
{
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET)
	{
		std::cout << "Socket creation failed: " << WSAGetLastError() << '\n';
		return -1;
	}
	return 1;
}

i32 Net::TCPServer::InitClientSocket()
{
	i32 clientAddrLen = sizeof(m_clientAddress);
	m_clientSocket = accept(m_serverSocket, reinterpret_cast<sockaddr*>(&m_clientAddress), &clientAddrLen);
	if (m_clientSocket == SOCKET_ERROR)
	{
		std::cout << "Accept failed\n";
		Net::close(m_serverSocket);
		return -1;
	}
	std::cout << "Client connected.\n";

	return 1;
}

i32 Net::TCPServer::InitServerAddress()
{
	m_serverAddress.sin_family = AF_INET;
	m_serverAddress.sin_port = htons(8080);
	int bindAddressResult = inet_pton(AF_INET, m_ip.c_str(), &m_serverAddress.sin_addr);
	if (bindAddressResult == SOCKET_ERROR)
	{
		std::cout << "binding inet_pton Failed\n";
		return -1;
	}
	int bindResult = bind(m_serverSocket, reinterpret_cast<sockaddr*>(&m_serverAddress), sizeof(m_serverAddress));
	if (bindResult == SOCKET_ERROR)
	{
		std::cout << "Bind failed\n";
		Net::close(m_serverSocket);
		return -1;
	}
	return 1;
}

i32 Net::TCPServer::InitListener()
{
	int listenResult = listen(m_serverSocket, SOMAXCONN);
	if (listenResult == SOCKET_ERROR)
	{
		std::cout << "Listen failed\n";
		closesocket(m_serverSocket);
		return -1;
	}
	std::cout <<"Server is listening on port " << m_serverAddress.sin_port << "...\n";

	return 1;

}

i32 Net::TCPServer::ReceiveBytes()
{
	constexpr size_t bufferSize = 512;
	std::vector<char> buffer;
	buffer.reserve(bufferSize);
	i32 bytesReceived = recv(m_clientSocket, buffer.data(), sizeof(buffer), 0);
	if (bytesReceived <= 0) {
		return -1;
	}
	buffer[bytesReceived] = '\0';  // Null-terminate the received data
	std::cout << "Received from client: " << buffer.data() << '\n';
	return 1;
}

i32 Net::TCPServer::ReceiveResponse()
{
	const std::string response = "Hello from server";
	int sendRes = send(m_clientSocket, response.c_str(), response.size(), 0);
	if (sendRes == SOCKET_ERROR)
	{
		std::cout << "Failed to send response!\n";
		Net::close(m_serverSocket);
		return -1;
	}
	return 1;
}

Net::TCPServer::~TCPServer()
{
	Net::close(m_clientSocket);
	Net::close(m_serverSocket);
	WSACleanup();
}
