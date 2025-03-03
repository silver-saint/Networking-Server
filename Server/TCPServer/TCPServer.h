#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "../Types/types.h"
#include "../NetworkTypes/NetworkingTypes.h"

namespace Net
{

	class TCPServer
	{
	public:
		TCPServer(const std::string& address, i16 port) noexcept;
		TCPServer(const TCPServer& other) = delete;
		TCPServer(TCPServer&& other) noexcept;
		void Init();
#if _WIN32
		[[nodiscard]] i32 InitWinSocket();
#endif
		[[nodiscard]] i32 InitServerSocket();
		[[nodiscard]] i32 InitClientSocket();
		[[nodiscard]] i32 InitServerAddress();
		[[nodiscard]] i32 InitListener();
		[[nodiscard]] i32 ReceiveBytes();
		[[nodiscard]] i32 ReceiveResponse();
		~TCPServer();

	private:
		socket_t serverSocket;
		socket_t clientSocket;
		sockaddr_in serverAddress;
		sockaddr_in clientAddress;
		std::string ip;
		i16 port;
	};
}