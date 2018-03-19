#pragma once

#include "OSCMessage.h"

namespace OSC {
	class MessageConsumer
	{
	public:
		virtual const char * pattern() = 0;
		virtual void callbackMessage(Message *) = 0;
	};

	template <typename Enum, class intSize>
	class StructMessageConsumer : public MessageConsumer
	{
	private:
		unsigned char ** _stagedStructs;
		int * _stagedStructSizes;

		int _structsReserved = 0;

		void _reserve(int count) {
			unsigned char ** tempStructs;
			int * tempStructSizes;

			if (_structsReserved > 0) {
				tempStructs = new unsigned char*[_structsReserved];
				tempStructSizes = new int[_structsReserved];

				memcpy(tempStructs, _stagedStructs, sizeof(unsigned char *) * _structsReserved);
				memcpy(tempStructSizes, _stagedStructSizes, sizeof(int *) * _structsReserved);

				delete _stagedStructs;
				delete _stagedStructSizes;
			}

			_structsReserved = _structsReserved + count;

			_stagedStructs = new unsigned char*[_structsReserved];
			_stagedStructSizes = new int[_structsReserved];

			if (_structsReserved - count > 0) {
				memcpy(_stagedStructs, tempStructs, sizeof(unsigned char *) * (_structsReserved - count));
				memcpy(_stagedStructSizes, tempStructSizes, sizeof(int *) * (_structsReserved - count));

				delete[] tempStructs;
				delete[] tempStructSizes;
			}
		}

	public:
		StructMessageConsumer(int mappingNumber) {
			reserveMappings(mappingNumber);
		}

		~StructMessageConsumer() {
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
			int d = offset;

			uint32_t intValue;
			float floatValue;

			int i = 0;

			while(i < structSize) {
				switch (message->getDataType(d)) {

				case DataType::Float:
					floatValue = message->getFloat(d);
					memcpy(dataStruct + i, &floatValue, 4);

					i += 4;
					break;

				case DataType::Integer:
				default:
					intValue = message->getInt(d);
					memcpy(dataStruct + i, &intValue, sizeof(intSize));

					i += sizeof(intSize);
					break;
				}

				++d;
			}
		}

		void reserveMappings(int number) {
			if (number > _structsReserved) {
				_reserve(number - _structsReserved);
			}
		}

		template <typename Struct>
		void addEnumToStructMapping(Enum valueOfFirstInt, Struct * dataStruct) {
			int firstValue = (int)valueOfFirstInt;

			reserveMappings(firstValue + 1);

			if (firstValue < _structsReserved) {
				_stagedStructSizes[firstValue] = sizeof(Struct);
				_stagedStructs[firstValue] = (unsigned char*)dataStruct;
			}
		}
	};
}