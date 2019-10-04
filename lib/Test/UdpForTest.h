#pragma once

#include <PrintForTest.h>
#include <cstring>

typedef int IPAddress;

class UDP : public Print
{
public:
	IPAddress ip;

	void beginPacket(IPAddress ip, int port)
	{
	}

	void endPacket()
	{
	}

	IPAddress remoteIP()
	{
		return ip;
	}

	int parsePacket()
	{
		return dataCountInBuffer;
	}

	void read(char *buffer, const int size)
	{
		if (dataCountInBuffer >= size)
		{
			memcpy(buffer, printBuffer, size);
		}
	}
};