#pragma once

// mock the IPAddress
#ifdef _MSC_VER
typedef int IPAddress;
#endif;

class UDP : public Print {
public:
	IPAddress ip;

	void beginPacket(IPAddress ip, int port) {

	}

	void endPacket() {

	}

	IPAddress remoteIP() {
		return ip;
	}

	int parsePacket() {
		return bufferSize;
	}

	void read(char * buffer, int size) {
		if (this->bufferSize > 0) {
			memcpy_s(buffer, size, this->buffer, this->bufferSize);
		}
	}

	void flush() {
		delete[] buffer;
		bufferSize = 0;
	}
};