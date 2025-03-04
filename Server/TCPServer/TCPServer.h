#pragma once

#include "../Networking/NetworkingTypes.h"
#include "../Networking/Socket.h"
namespace Net
{

	class TCPServer : public Socket
	{
	public:
		TCPServer(const std::string& address, i16 port) noexcept;
		TCPServer(const TCPServer& other) = delete;
		TCPServer(TCPServer&& other) noexcept;
		void Init();
#if _WIN32
		 i32 InitWinSocket();
#endif
		 i32 CreateSocket() override;
		 i32 AcceptConnections();
		 i32 BindSocket();
		 i32 ListenToPort();
		 i32 Receive() override;
		 i32 Send();
		virtual ~TCPServer();

	private:
		socket_t m_serverSocket;
		socket_t m_clientSocket;
		sockaddr_in m_serverAddress;
		sockaddr_in m_clientAddress;
		std::string m_ip;
		i16 m_port;
	};
}