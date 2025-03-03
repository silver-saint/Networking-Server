#pragma once

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
		socket_t m_serverSocket;
		socket_t m_clientSocket;
		sockaddr_in m_serverAddress;
		sockaddr_in m_clientAddress;
		std::string m_ip;
		i16 m_port;
	};
}