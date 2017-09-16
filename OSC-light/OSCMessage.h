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
		static inline int _padSize(int bytes) { return (4 - (bytes & 03)) & 3; }
		

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
		~Message() {
			if (reservedCount > 0) {
				delete[] data;
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
		// Deletes the current OSCDatas and creates new ones. Use before add<T>()!
		void reserve(int count) {
			if (reservedCount > 0) {
				delete[] data;
			}
			reservedCount += count;
			data = new Data[reservedCount];
		}

		// Reserves the specified amount of OSCData elements to avoid multiple reallocations of the buffer.
		// Deletes the current OSCDatas and creates new ones, only when current reserved count is smaller than given count. Use before add<T>()!
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

		// Gets the value at the given data position.
		float getFloat(int position) {
			if (position < dataCount) {
				return data[position].getFloat();
			}

			return 0.0;
		}
		// Gets the value at the given data position.
		int getInt(int position) {
			if (position < dataCount) {
				return data[position].getInt();
			}

			return 0;
		}
		DataType getDataType(int position) {
			if (position < dataCount) {
				return data[position].type;
			}

			return (DataType)0;
		}

		template <typename T>
		void add(T datum) {
			if (dataCount >= reservedCount) {
				reserve(1);
			}

			data[dataCount++].set(datum);
		}

		// Sets the value at the given position.
		template <typename T>
		void set(int location, T datum) {
			if (location < dataCount) {
				data[location].set(datum);
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
		void send(Print * print) {
			char nullChar = '\0';
			int addressLength = strlen(address) + 1;
			int addressPadding = _padSize(addressLength);
			int typePadding = _padSize(dataCount + 1);
			if (typePadding == 0) {
				typePadding = 4;
			}

			int bufferPosition = 0;
			int bufferSize = addressLength + addressPadding + 1 + dataCount + typePadding + (dataCount * 4);

			// TODO: create this buffer once
			char * buffer = new char[bufferSize];

			strcpy(buffer, address);
			bufferPosition = addressLength;

			while (addressPadding--) {
				buffer[bufferPosition++] = nullChar;
			}

			buffer[bufferPosition++] = ',';

			for (int i = 0; i < dataCount; ++i) {
				switch (data[i].type) {
				case DataType::Integer:
					buffer[bufferPosition++] = 'i';
					break;
				case DataType::Float:
					buffer[bufferPosition++] = 'f';
					break;
				}
			}

			while (typePadding--) {
				buffer[bufferPosition++] = nullChar;
			}

			for (int i = 0; i < dataCount; ++i) {
				data[i].outputOSCData(buffer + bufferPosition);

				bufferPosition += 4;
			}

			print->write(buffer, bufferSize);

			delete[] buffer;
		}

		// Fills the data with the given data buffer.
		// To improve performance, do not destroy instances of OSCMessage but use fill() multiple times.
		void process() {
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

			for (int i = 0; i < newDataCount; ++i) {
				data[i].inputOSCData(processBuffer + dataStart + (4 * i));

				switch (subBuffer[i + 1]) {
				case 'i':
					data[i].type = DataType::Integer;
					break;
				case 'f':
					data[i].type = DataType::Float;
					break;
				}
			}

			dataCount = newDataCount;
		}

		// Reserves the amount of data for use in the fill() method.
		void reserveForProcess(int dataLength) {
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