#pragma once

#include <Arduino.h>
#include <cstring>

class TestStream : public Stream
{
public:
	IPAddress ip;

	int pointer = 0;
	char data[37] = "/TEST\0\0\0,iiiii\0\0\0\0\0\x1\0\0\0\x2\0\0\0\x3\0\0\0\x4\0\0\0\x5";

	void flush()
	{
	}

	int peek()
	{
		return 0;
	}

	int available()
	{
		pointer = 0;
		return 36;
	}

	int read()
	{
		return data[pointer++];
	}

	size_t write(uint8_t data)
	{
		return 0;
	}

	size_t write(const uint8_t *buffer, size_t size)
	{
		memcpy(data, buffer, size);

		return 0;
	}
};