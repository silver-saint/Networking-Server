#include "TCPServer.h"

Net::TCPServer::TCPServer(const std::string& address, i16 port)
	: serverSocket{0}, clientSocket{0}, serverAddress{0},clientAddress{0}, ip{address}, port{port} {}

Net::TCPServer::TCPServer(TCPServer&& other) noexcept
	: serverSocket {std::move(other.serverSocket)},
	  clientSocket {std::move(other.clientSocket)},
	  serverAddress {std::move(other.serverAddress)},
	  clientAddress {std::move(other.clientAddress)},
	  ip {std::move(other.ip)}, 
	  port{ std::move(other.port) } {}

void Net::TCPServer::Init()
{
#if _WIN32
	InitWinSocket();
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
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		std::cout << "Socket creation failed: " << WSAGetLastError() << '\n';
		return -1;
	}
	return 1;
}

i32 Net::TCPServer::InitClientSocket()
{
	i32 clientAddrLen = sizeof(clientAddress);
	clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddrLen);
	if (clientSocket == SOCKET_ERROR)
	{
		std::cout << "Accept failed\n";
		Net::close(serverSocket);
		return -1;
	}
	std::cout << "Client connected.\n";

	return 1;
}

i32 Net::TCPServer::InitServerAddress()
{
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	int bindAddressResult = inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr);
	if (bindAddressResult == SOCKET_ERROR)
	{
		std::cout << "binding inet_pton Failed\n";
		return -1;
	}
	int bindResult = bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
	if (bindResult == SOCKET_ERROR)
	{
		std::cout << "Bind failed\n";
		Net::close(serverSocket);
		return -1;
	}
	return 1;
}

i32 Net::TCPServer::InitListener()
{
	int listenResult = listen(serverSocket, SOMAXCONN);
	if (listenResult == SOCKET_ERROR)
	{
		std::cout << "Listen failed\n";
		closesocket(serverSocket);
		return -1;
	}
	std::cout <<"Server is listening on port " << serverAddress.sin_port << "...\n";

	return 1;

}

i32 Net::TCPServer::ReceiveBytes()
{
	constexpr size_t bufferSize = 512;
	std::vector<char> buffer;
	buffer.reserve(bufferSize);
	i32 bytesReceived = recv(clientSocket, buffer.data(), sizeof(buffer), 0);
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
	int sendRes = send(clientSocket, response.c_str(), response.size(), 0);
	if (sendRes == SOCKET_ERROR)
	{
		std::cout << "Failed to send response!\n";
		Net::close(serverSocket);
		return -1;
	}
	return 1;
}

Net::TCPServer::~TCPServer()
{
	Net::close(clientSocket);
	Net::close(serverSocket);
	WSACleanup();
}
