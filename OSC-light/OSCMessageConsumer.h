#pragma once

#include "OSCMessage.h"

namespace OSC {
	class IMessageConsumer
	{
	public:
		virtual const char * pattern() = 0;
		virtual void callbackMessage(Message *) = 0;
	};

	template <typename Enum>
	class AMessageConsumerUsingStructs : IMessageConsumer
	{
	private:
		unsigned char ** _stagedStructs;
		int * _stagedStructSizes;

		int _structsReserved = 0;

	public:
		AMessageConsumerUsingStructs() {
		}

		~AMessageConsumerUsingStructs() {
			if (_structsReserved > 0) {
				delete _stagedStructs;
				delete _stagedStructSizes;
			}
		}

		virtual void callbackEnum(Enum messageType) = 0;

		void callbackMessage(Message * message) {
			int value = message->getInt(0);
			if (value < _structsReserved) {
				
				readToStruct(message, _stagedStructs[value], _stagedStructSizes[value], 1);
				callbackEnum((Enum)value);

			}
		}

		// Writes the values in data to the given struct type and return it
		void readToStruct(Message * message, unsigned char * dataStruct, int structSize, int offset = 0) {
			int size = (int)(structSize / 4);
			int offsetInBytes = offset * 4;

			int d = offset;

			int intValue;
			float floatValue;

			for (int i = 0; i < structSize; i += 4) {
				switch (message->getDataType(d)) {

				case DataType::Integer:
					intValue = message->getInt(d);
					memcpy(dataStruct + i, &intValue, 4);
					break;

				case DataType::Float:
					floatValue = message->getFloat(d);
					memcpy(dataStruct + i, &floatValue, 4);
					break;

				default:
					intValue = 0;
					memcpy(dataStruct + i, &intValue, 4);
					break;
				}

				++d;
			}
		}

		void reserveAtLeast(int numberOfStructs) {
			if (_structsReserved < numberOfStructs) {
				if (_structsReserved > 0) {
					delete _stagedStructs;
					delete _stagedStructSizes;
				}

				_structsReserved = numberOfStructs;
			}

			_stagedStructs = new unsigned char*[_structsReserved];
			_stagedStructSizes = new int[_structsReserved];
		}

		template <typename Struct>
		void stageStruct(Enum valueOfFirstInt, Struct * dataStruct, int structSize) {
			int firstValue = (int)valueOfFirstInt;

			if (firstValue < _structsReserved) {
				_stagedStructSizes[firstValue] = structSize;
				_stagedStructs[firstValue] = (unsigned char*)dataStruct;
			}
		}
	};
}