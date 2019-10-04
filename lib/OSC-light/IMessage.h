#pragma once

#ifdef UNIT_TEST
#ifdef ARDUINO
#include <Arduino.h>
#else
#include <UdpForTest.h>
#include <StreamForTest.h>
#endif
#else
#include <Print.h>
#endif

#include "OSCData.h"
#include "OSCMatch.h"

namespace OSC
{

class IMessage
{
public:
	// Char array containing the address
	char *address;

	virtual ~IMessage()
	{
		delete address;
	}

	// Sets the address of the message.
	void setAddress(const char *newAddress)
	{
		if (address != nullptr)
		{
			delete[] address;
		}

		address = new char[strlen(newAddress) + 1];
		strcpy(address, newAddress);
	}

	// Boolean to evaluate whether the message should be send.
	bool isSendableMessage()
	{
		return _validData;
	}

	// Sets whether the data of the message is valid
	void setValidData(bool valid)
	{
		_validData = valid;
	}

	// Sends the data using the given Print object.
	virtual void send(Print *, const bool platformIsBigEndian = _isBigEndian()) = 0;

	// Fills the data with the given data buffer.
	// To improve performance, do not destroy instances of OSCMessage but use process() multiple times.
	virtual bool process(const int messageSize, const char *processBuffer, char *subBuffer, const bool platformIsBigEndian = _isBigEndian()) = 0;

protected:
	bool _validData = true;

	static bool _isBigEndian()
	{
		const int one = 1;
		const char sig = *(char *)&one;

		return (sig == 0);
	}
	static int _padSize(int bytes)
	{
		return (4 - (bytes & 03)) & 3;
	}
};
} // namespace OSC