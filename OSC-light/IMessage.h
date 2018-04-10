#pragma once

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

	enum MessageType {
		Regular = 1,
		Struct = 2
	};

	class IMessage
	{
	public:
		// TODO: convert this to a virtual method which implements method for determining valid serial buffer length
		MessageType messageType;

		// Length of the process buffer
		int bufferLength = 0;

		// Process buffer for writing raw OSC data
		char * processBuffer;

		// Sub process buffer for writing chucks of buffered raw OSC data
		char * subBuffer;

		// Char array containing the address 
		char * address;

		virtual ~IMessage() {
			if (bufferLength > 0) {
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

		// Reserves the amount of data for use in the process() / send() method.
		void reserveBuffer(int dataLength) {
			if (dataLength > bufferLength) {
				if (bufferLength > 0) {
					memcpy(subBuffer, processBuffer, bufferLength);
					delete[] processBuffer;
				}

				processBuffer = new char[dataLength + 4];

				// restore process buffer to allow for further processing
				if (bufferLength > 0) {
					memcpy(processBuffer, subBuffer, bufferLength);
					delete[] subBuffer;
				}

				bufferLength = dataLength + 4;

				subBuffer = new char[bufferLength];
			}
		}

		// Sends the data using the given Print object.
		virtual void send(Print *, bool platformIsBigEndian = _isBigEndian()) = 0;

		// Fills the data with the given data buffer.
		// To improve performance, do not destroy instances of OSCMessage but use process() multiple times.
		virtual void process(bool platformIsBigEndian = _isBigEndian()) = 0;
	protected:
		bool _validData = true;

		static bool _isBigEndian() {
			const int one = 1;
			const char sig = *(char*)&one;

			return (sig == 0);
		}
		static int _padSize(int bytes) { 
			return (4 - (bytes & 03)) & 3; 
		}
	};
}