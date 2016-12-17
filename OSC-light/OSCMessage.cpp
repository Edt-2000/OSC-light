#pragma once

#include "OSCMessage.h"
#include "OSCMatch.h"

OSC::Message::Message() {
	address = NULL;
}

OSC::Message::~Message() {
	if (reservedCount > 0) {
		delete[] data;
	}
}

void OSC::Message::setAddress(const char * newAddress) {
	if (address != nullptr) {
		delete[] address;
	}

	address = new char[strlen(newAddress) + 1];
	strcpy(address, newAddress);
}

void OSC::Message::reserve(int count) {
	if (reservedCount > 0) {
		delete[] data;
	}
	reservedCount += count;
	data = new Data[reservedCount];
}

void OSC::Message::reserveAtLeast(int count) {
	if (reservedCount < count) {
		reserve(count - reservedCount);
	}
}

void OSC::Message::empty() {
	for (int i = 0; i < reservedCount; ++i) {
		data[i].empty();
	}
	dataCount = 0;
}

void OSC::Message::send(Print * p) {
	char nullChar = '\0';
	int addressLength = strlen(address) + 1;
	int addressPadding = _padSize(addressLength);
	int typePadding = _padSize(dataCount + 1);
	if (typePadding == 0) {
		typePadding = 4;
	}

	int bufferPosition = 0;
	int bufferSize = addressLength + addressPadding + 1 + dataCount + typePadding + (dataCount * 4);

	char * buffer = new char[bufferSize];

	strcpy(buffer, address);
	bufferPosition = addressLength;

	while (addressPadding--) {
		buffer[bufferPosition++] = nullChar;
	}

	buffer[bufferPosition++] = ',';

	for (int i = 0; i < dataCount; ++i) {
		switch (data[i].type) {
		case DataType::i:
			buffer[bufferPosition++] = 'i';
			break;
		case DataType::f:
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

	p->write(buffer, bufferSize);

	delete[] buffer;
}

void OSC::Message::reserveForProcess(int dataLength)
{
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

void OSC::Message::process()
{
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
			data[i].type = DataType::i;
			break;
		case 'f':
			data[i].type = DataType::f;
			break;
		}
	}

	dataCount = newDataCount;
}
