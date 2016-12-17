#pragma once

// TODO: do not include the things required for testing here
#ifdef _MSC_VER
#include "../OSC-lightTest/Print.h"
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
	public:
		Message();
		~Message();

		// Sets the address of the message.
		void setAddress(const char * address);

		// Reserves the specified amount of OSCData elements to avoid multiple reallocations of the buffer.
		// Deletes the current OSCDatas and creates new ones. Use before add<T>()!
		void reserve(int count);

		// Reserves the specified amount of OSCData elements to avoid multiple reallocations of the buffer.
		// Deletes the current OSCDatas and creates new ones, only when current reserved count is smaller than given count. Use before add<T>()!
		void reserveAtLeast(int count);

		// Removes all data from the OSCData buffer.
		void empty();

		// Gets the value at the given data position.
		float getFloat(int position) {
			if (position < dataCount) {
				return data[position].data.f;
			}

			return 0.0;
		}
		// Gets the value at the given data position.
		int getInt(int position) {
			if (position < dataCount) {
				return data[position].data.i;
			}

			return 0;
		}

		// Sets the value in a new data position. 
		// To improve performance, this new position should be reserved first.
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
			return _matchHelper.isMatch(address, pattern);
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
		void send(Print * print);

		// Fills the data with the given data buffer.
		// To improve performance, do not destroy instances of OSCMessage but use fill() multiple times.
		void process();

		// Reserves the amount of data for use in the fill() method.
		void reserveForProcess(int dataLength);

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

	private:

		bool _validData = true;
		static inline int _padSize(int bytes) { return (4 - (bytes & 03)) & 3; }
	};
};