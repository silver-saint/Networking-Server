#pragma once
#include "../Networking/NetworkingTypes.h"
#include "../Networking/Socket.h"
namespace Net
{
	
	class TCPClient : public Socket
	{
	public:
		TCPClient(const std::string& address, i16 port) noexcept;
		TCPClient(const TCPClient& other) = delete;
		TCPClient(TCPClient&& other) noexcept;
		void Init();
#ifdef _WIN32
		i32 InitWinSocket();
#endif
		i32 CreateSocket() override;
		i32 Receive() override;
		i32 Send() override;
		i32 Connect();
		i32 BindAddress();
		

	private:
		//static void ReceiveLoop(TCPClient* client);
	private:
		socket_t m_serverSocket;
		sockaddr_in m_serverAddress;
		std::string m_ip;
		std::vector<char> m_msgBuffer;
		i16 m_port;
	};
}