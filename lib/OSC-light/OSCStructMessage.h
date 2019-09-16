#pragma once

#ifdef UNIT_TEST 
#ifndef ARDUINO
#include <PrintForTest.h>
#endif
#else
#include <Print.h>
#endif

#include "OSCMatch.h"
#include "IMessage.h"

namespace OSC {

	template <typename Struct, class intSize>
	class StructMessage : public IMessage
	{
	public:
		Struct messageStruct = Struct();

		StructMessage() {
			address = NULL;
		}

		// Sends the data using the given Print object.s
		void send(Print * print, bool platformIsBigEndian = _isBigEndian()) {

			int reservedCount = (sizeof(Struct) / sizeof(intSize));

			char nullChar = '\0';
			int addressLength = strlen(address) + 1;
			int addressPadding = _padSize(addressLength);
			int typePadding = _padSize(reservedCount + 1);
			if (typePadding == 0) {
				typePadding = 4;
			}

			int bufferPosition = 0;
			int bufferSize = addressLength + addressPadding + 1 + reservedCount + typePadding + (reservedCount * 4);

			// make sure processBuffer is big enough
			reserveBuffer(bufferSize);

			strcpy(processBuffer, address);

			bufferPosition = addressLength;

			while (addressPadding--) {
				processBuffer[bufferPosition++] = nullChar;
			}

			processBuffer[bufferPosition++] = ',';

			for (int i = 0; i < reservedCount; ++i) {

				processBuffer[bufferPosition++] = 'i';

			}

			while (typePadding--) {
				processBuffer[bufferPosition++] = nullChar;
			}

			char readBuffer[4] = { 0, 0, 0, 0 };
			if (!platformIsBigEndian) {
				for (int i = 0; i < reservedCount; ++i) {
					memcpy(readBuffer, ((char*)&messageStruct) + (i * sizeof(intSize)), sizeof(intSize));

					for (int d = 0; d < 4; ++d) {
						processBuffer[bufferPosition + d] = readBuffer[3 - d];
					}

					bufferPosition += 4;
				}
			}
			else {
				for (int i = 0; i < reservedCount; ++i) {
					if (sizeof(intSize) < 4) {
						memcpy(processBuffer + bufferPosition, readBuffer, 4);
					}

					memcpy(processBuffer + bufferPosition, ((char*)&messageStruct) + (i * sizeof(intSize)), sizeof(intSize));

					bufferPosition += 4;
				}
			}

			print->write(processBuffer, bufferSize);

		}

		// Checks if the data in process buffer is valid
		int determineSize(int size) {
			return size >= sizeof(Struct) ? sizeof(Struct) : 0;
		}

		// Fills the data with the given data buffer.
		// To improve performance, do not destroy instances of OSCMessage but use process() multiple times.
		bool process(bool platformIsBigEndian = _isBigEndian()) {
			
			int addressLength = 0;
			int typeStart = 0;
			int typeLength = 0;
			int newDataCount = sizeof(Struct) / sizeof(intSize);
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

			if (!platformIsBigEndian) {
				char buffer[4];

				// reverse all the data in the process buffer
				for (int i = 0; i < newDataCount; ++i) {
					int offset = dataStart + (4 * i);

					memcpy(buffer, processBuffer + offset, 4);

					for (int d = 0; d < 4; ++d) {
						processBuffer[offset + d] = buffer[3 - d];
					}
				}
			}

			for (int i = 0; i < newDataCount; i++) {
				memcpy(((char*)&messageStruct) + (i * sizeof(intSize)), processBuffer + dataStart + (i * 4), sizeof(intSize));
			}

			// for now, every message is ok
			return true;
		}
	};

}