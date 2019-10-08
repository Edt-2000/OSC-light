#pragma once

#ifdef UNIT_TEST
#ifdef ARDUINO
#include <Arduino.h>
#else
#include <PrintForTest.h>
#endif
#else
#include <Print.h>
#endif

#include "OSCMatch.h"
#include "IMessage.h"
#include "EndianMemcpy.h"

#include <memory>
#include <iostream>

namespace OSC
{

template <typename Struct, class intSize>
class StructMessage : public IMessage
{
public:
	Struct messageStruct = Struct();

	StructMessage()
	{
		address = NULL;
	}

	// Sends the data using the given Print object.s
	void send(Print *print, const bool platformIsBigEndian = _isBigEndian())
	{
		constexpr int reservedCount = (sizeof(Struct) / sizeof(intSize));
		constexpr char nullChar = '\0';

		const int addressLength = strlen(address) + 1;
		int addressPadding = _padSize(addressLength);
		int typePadding = _padSize(reservedCount + 1);
		if (typePadding == 0)
		{
			typePadding = 4;
		}

		int bufferPosition = 0;
		const int bufferSize = addressLength + addressPadding + 1 + reservedCount + typePadding + (reservedCount * 4);

		char processBuffer[bufferSize];

		strcpy(processBuffer, address);

		bufferPosition = addressLength;

		while (addressPadding--)
		{
			processBuffer[bufferPosition++] = nullChar;
		}

		processBuffer[bufferPosition++] = ',';

		for (int i = 0; i < reservedCount; ++i)
		{
			processBuffer[bufferPosition++] = 'i';
		}

		while (typePadding--)
		{
			processBuffer[bufferPosition++] = nullChar;
		}

		char readBuffer[4] = {0, 0, 0, 0};

		for (int i = 0; i < reservedCount; ++i)
		{
			if (sizeof(intSize) < 4)
			{
				memcpy(processBuffer + bufferPosition, readBuffer, 4);
			}

			memcpy(processBuffer + bufferPosition, ((char *)&messageStruct) + (i * sizeof(intSize)), sizeof(intSize), platformIsBigEndian);

			bufferPosition += 4;
		}

		print->write(processBuffer, bufferSize);
	}

	// Fills the data with the given data buffer.
	// To improve performance, do not destroy instances of OSCMessage but use process() multiple times.
	bool process(const int messageSize, const char *processBuffer, char *subBuffer, const bool platformIsBigEndian = _isBigEndian())
	{
		int addressLength = 0;
		int typeStart = 0;
		int typeLength = 0;
		constexpr auto newDataCount = sizeof(Struct) / sizeof(intSize);
		int dataStart = 0;

		// address
		strcpy(subBuffer, processBuffer);
		addressLength = strlen(subBuffer) + 1;

		setAddress(subBuffer);

		// types
		typeStart = addressLength + _padSize(addressLength);
		strcpy(subBuffer, processBuffer + typeStart);

		// type values
		typeLength = strlen(subBuffer) + 1;
		dataStart = typeStart + typeLength + _padSize(typeLength);

		for (int i = 0; i < min(newDataCount, messageSize / sizeof(intSize)); i++)
		{
			memcpy(((char *)&messageStruct) + (i * sizeof(intSize)), processBuffer + dataStart + (i * 4), sizeof(intSize), platformIsBigEndian);
		}

		return true;
	}
};

} // namespace OSC