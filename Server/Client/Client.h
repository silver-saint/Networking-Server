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
		Client(const Client&& other) noexcept;

		void Init();
		i32 Connect();
	private:
		socket_t socket;
		std::string ip;
		i16 port;
	};
}