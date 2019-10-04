#pragma once

#include <PrintForTest.h>
#include <cstring>

typedef int IPAddress;

class Stream : public Print
{
public:
	IPAddress ip;

	IPAddress remoteIP()
	{
		return ip;
	}

	int available()
	{
		return dataCountInBuffer;
	}

	void println(char *string) {}

	void readBytes(char *buffer, const int size)
	{
		if (dataCountInBuffer >= size)
		{
			memcpy(buffer, printBuffer, size);
		}
	}
};