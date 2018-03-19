#pragma once

// TODO: do not include the things required for testing here
#ifdef _MSC_VER
#include "../OSC-lightUnitTest/Print.h"
#else
#include <Print.h>
#endif

#include "OSCData.h"
#include "OSCMatch.h"

namespace OSC {

	// static helpers
	static Match _matchHelper = Match();

	class Message
	{
	private:
		bool _validData = true;
		static int _padSize(int bytes) { return (4 - (bytes & 03)) & 3; }
		static bool _isBigEndian() {
			const int one = 1;
			const char sig = *(char*)&one;

			return (sig == 0);
		}

	public:
		// Length of the process buffer
		int bufferLength = 0;

		// Process buffer for writing raw UDP data
		char * processBuffer;

		// Sub process buffer for writing chucks of buffered raw UDP data
		char * subBuffer;

		// Char array containing the address 
		char * address;

		// Number of data elements in data array
		int dataCount = 0;

		// Number of data elements available in data array
		int reservedCount = 0;

		// Data buffer containing every OSC variable of this message
		Data * data;

		Message() {
			address = NULL;
		}
		Message(int dataNumber) {
			address = NULL;

			reserveAtLeast(dataNumber);
		}
		~Message() {
			if (reservedCount > 0) {
				delete[] data;
			}
			if(bufferLength > 0) {
				delete[] processBuffer;
				delete[] subBuffer;
			}
		}

		// Sets the address of the message.
		void setAddress(const char * newAddress) {
			if (address != nullptr) {
				delete[] address;
			}

			address = new char[strlen(newAddress) + 1];
			strcpy(address, newAddress);
		}

		// Reserves the specified amount of OSCData elements to avoid multiple reallocations of the buffer.
		void reserve(int count) {
			Data * tempBuffer;

			if (reservedCount > 0) {
				tempBuffer = new Data[reservedCount];
				memcpy(tempBuffer, data, reservedCount * sizeof(Data));
				delete[] data;
			}

			reservedCount += count;
			data = new Data[reservedCount];

			if (reservedCount > count) {
				memcpy(data, tempBuffer, (reservedCount - count) * sizeof(Data));
				delete[] tempBuffer;
			}
		}

		// Reserves the specified amount of OSCData elements to avoid multiple reallocations of the buffer.
		// Deletes the current OSCDatas and creates new ones, only when current reserved count is smaller than given count. Use before multiple adds or sets!
		void reserveAtLeast(int count) {
			if (reservedCount < count) {
				reserve(count - reservedCount);
			}
		}

		// Removes all data from the OSCData buffer.
		void empty() {
			for (int i = 0; i < reservedCount; ++i) {
				data[i].empty();
			}
			dataCount = 0;
		}

		float getFloat(int position) {
			if (position < reservedCount) {
				return data[position].getFloat();
			}

			return 0.0;
		}
		uint32_t getInt(int position) {
			if (position < reservedCount) {
				return data[position].getInt();
			}

			return 0;
		}
		DataType getDataType(int position) {
			if (position < reservedCount) {
				return data[position].getDataType();
			}

			return (DataType)0;
		}

		// Add the given value the next free slot
		void addInt(uint32_t datum) {
			reserveAtLeast(dataCount + 1);
			data[dataCount++].setInt(datum);
		}

		// Add the given value the next free slot
		void addFloat(float datum) {
			reserveAtLeast(dataCount + 1);
			data[dataCount++].setFloat(datum);
		}

		// Adds the given struct from the netxt free slot
		template <typename T>
		void add(T * newData, DataType * types = NULL) {
			reserveAtLeast(dataCount + 1 + (sizeof(T) / 4));
			
			set<T>(dataCount, newData, types);

			dataCount += sizeof(T) / 4;
		}

		// Sets the value at the given position.
		void setInt(int location, uint32_t datum) {
			reserveAtLeast(location + 1);
			data[location].setInt(datum);
		}

		// Sets the value at the given position.
		void setFloat(int location, float datum) {
			reserveAtLeast(location + 1);
			data[location].setFloat(datum);
		}

		// Sets the given struct from the given position
		template <typename T>
		void set(int startLocation, T * newData, DataType * types = NULL) {
			reserveAtLeast(startLocation + 1 + (sizeof(T) / 4));

			int d = 0;

			if (types != NULL) {
				for (unsigned int i = 0; i < sizeof(T); i += 4) {
					data[startLocation + d].set((unsigned char *)newData + i, types[startLocation + d]);

					++d;
				}
			}
			else {
				for (unsigned int i = 0; i < sizeof(T); i += 4) {
					data[startLocation + d].set((unsigned char *)newData + i);

					++d;
				}
			}
		}

		// Evaluates wheter the given pattern is a valid route for the message.
		bool isValidRoute(const char * pattern) {
			// the address is of the message is the pattern to match
			return _matchHelper.isMatch(pattern, address);
		}

		// Boolean to evaluate whether the message should be send.
		bool isSendableMessage() {
			return _validData;
		}

		// Sets whether the data of the message is valid
		void setValidData(bool valid) {
			_validData = valid;
		}

		// Sends the data using the given Print object.
		void send(Print * print, bool platformIsBigEndian = _isBigEndian()) {

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
				switch (data[i].getDataType()) {
				case DataType::Float:
					processBuffer[bufferPosition++] = 'f';
					break;
				case DataType::Integer:
				default:
					processBuffer[bufferPosition++] = 'i';
					break;
				}
			}

			while (typePadding--) {
				processBuffer[bufferPosition++] = nullChar;
			}

			if (!platformIsBigEndian) {
				char readBuffer[4];
				for (int i = 0; i < reservedCount; ++i) {
					data[i].get(readBuffer);

					for (int d = 0; d < 4; ++d) {
						processBuffer[bufferPosition + d] = readBuffer[3 - d];
					}

					bufferPosition += 4;
				}
			}
			else {
				for (int i = 0; i < reservedCount; ++i) {
					data[i].get(processBuffer + bufferPosition);

					bufferPosition += 4;
				}
			}

			print->write(processBuffer, bufferSize);
		}

		// Fills the data with the given data buffer.
		// To improve performance, do not destroy instances of OSCMessage but use process() multiple times.
		void process(bool platformIsBigEndian = _isBigEndian()) {
			// make sure the message is empty
			empty();

			int addressLength = 0;
			int typeStart = 0;
			int typeLength = 0;
			int newDataCount = 0;
			int dataStart = 0;

			// address
			strcpy(subBuffer, processBuffer);
			addressLength = strlen(subBuffer) + 1;

			setAddress(subBuffer);

			// types
			typeStart = addressLength + _padSize(addressLength);
			strcpy(subBuffer, processBuffer + typeStart);
			newDataCount = strlen(subBuffer) - 1;

			reserveAtLeast(newDataCount);

			// type values
			typeLength = strlen(subBuffer) + 1;
			dataStart = typeStart + typeLength + _padSize(typeLength);

			if (!platformIsBigEndian) {
				char buffer[4];

				// reverse all the data in the process buffer
				for (int i = 0; i < newDataCount; ++i) {
					memcpy(buffer, processBuffer + dataStart + (4 * i), 4);

					for (int d = 0; d < 4; ++d) {
						processBuffer[dataStart + (4 * i) + d] = buffer[3 - d];
					}
				}
			}

			for (int i = 0; i < newDataCount; ++i) {
				switch (subBuffer[i + 1]) {
				case 'i':
					data[i].set((unsigned char *)processBuffer + dataStart + (4 * i), DataType::Integer);
					break;
				case 'f':
					data[i].set((unsigned char *)processBuffer + dataStart + (4 * i), DataType::Float);
					break;
				}
			}
			
			dataCount = newDataCount;
		}

		// Reserves the amount of data for use in the process() / send() method.
		void reserveBuffer(int dataLength) {
			if (dataLength > bufferLength) {
				if (bufferLength > 0) {
					delete[] processBuffer;
					delete[] subBuffer;
				}

				bufferLength = dataLength + 4;

				processBuffer = new char[bufferLength];
				subBuffer = new char[bufferLength];
			}
		}
	};
};