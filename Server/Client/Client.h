#pragma once
#include "../NetworkTypes/NetworkingTypes.h"
#include "../Types/types.h"
namespace Net
{
	
	class Client
	{
	public:
		Client(const std::string& address, i16 port) noexcept;
		Client(const Client& other) = delete;
		Client(Client&& other) noexcept;
		void Init();
		i32 Connect();
		i32 Send(const std::string& buffer, i32 len);
		void Close();

	private:
		static void ReceiveLoop(Client* client);
	private:
		socket_t m_socket;
		std::string m_ip;
		std::vector<char> m_msgBuffer;
		i16 m_port;
	};
}