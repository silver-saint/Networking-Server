#pragma once
#include "NetworkingTypes.h"
class Socket
{
public:
	virtual ~Socket() = default;
	 virtual i32 CreateSocket() = 0;
	 virtual i32 Send() = 0;
	 virtual i32 Receive() = 0;
};