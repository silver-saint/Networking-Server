#include "TCPServer.h"

TCPServer::TCPServer(const std::string& address, i16 port)
	: ip(address), port(port)
{
}

void TCPServer::Init()
{
#if _WIN32
	InitWinSocket();
#endif
	InitServerSocket();
	InitServerAddress();
	ConnectToServer();
	InitListener();
	InitClientSocket();
	ReceiveBytes();
	ReceiveResponse();
}

i32 TCPServer::InitWinSocket()
{
	WSADATA wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (Result != 0)
	{
		std::cout << "WSAStartup failed: " << Result << '\n';
		return -1;
	}
	std::cout << "Winsock initialized.\n";
	std::cout << "Current version: " << LOBYTE(wsaData.wVersion) << "." << HIBYTE(wsaData.wVersion) << std::endl;
	return 1;
}

i32 TCPServer::InitServerSocket()
{
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		std::cout << "Socket creation failed: " << WSAGetLastError() << '\n';
		return -1;
	}
	return 1;
}

i32 TCPServer::InitClientSocket()
{
	int32_t clientAddrLen = sizeof(clientAddress);
	clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddrLen);
	if (clientSocket == SOCKET_ERROR)
	{
		std::cout << "Accept failed\n";
		net::close(serverSocket);
		return -1;
	}
	std::cout << "Client connected.\n";

	return 1;
}

i32 TCPServer::ConnectToServer()
{
	int32_t connectionResult = connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
	if (connectionResult == SOCKET_ERROR)
	{
		std::cout << "Client connection failed: " << WSAGetLastError() << '\n';
		return -1;
	}
	return 1;
}

i32 TCPServer::InitServerAddress()
{
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	int bindAddressResult = inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr);
	if (bindAddressResult == SOCKET_ERROR)
	{
		std::cout << "binding inet_pton Failed\n";
		return -1;
	}
	int bindResult = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	if (bindResult == SOCKET_ERROR)
	{
		std::cout << "Bind failed\n";
		net::close(serverSocket);
		return -1;
	}
	return 1;
}

i32 TCPServer::InitListener()
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

i32 TCPServer::ReceiveBytes()
{
	constexpr size_t bufferSize = 512;
	std::vector<char> buffer;
	buffer.reserve(bufferSize);
	int32_t bytesReceived = recv(clientSocket, buffer.data(), sizeof(buffer), 0);
	if (bytesReceived <= 0) {
		return -1;
	}
	buffer[bytesReceived] = '\0';  // Null-terminate the received data
	std::cout << "Received from client: " << buffer.data() << '\n';
	return 1;
}

i32 TCPServer::ReceiveResponse()
{
	const std::string response = "Hello from server";
	int sendRes = send(clientSocket, response.c_str(), response.size(), 0);
	if (sendRes == SOCKET_ERROR)
	{
		std::cout << "Failed to send response!\n";
		net::close(serverSocket);
		return -1;
	}
	return 1;
}

TCPServer::~TCPServer()
{
	net::close(clientSocket);
	net::close(serverSocket);
	WSACleanup();
}
