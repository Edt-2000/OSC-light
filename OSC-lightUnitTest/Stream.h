#pragma once

// mock the IPAddress
#ifdef _MSC_VER
typedef int IPAddress;
#endif;

class Stream : public Print {
public:
	IPAddress ip;

	IPAddress remoteIP() {
		return ip;
	}

	int available() {
		return bufferSize;
	}

	void println(char * string) {}

	void readBytes(char * buffer, int size) {
		if (this->bufferSize > 0) {
			memcpy_s(buffer, size, this->buffer, this->bufferSize);
		}
	}

	void flush() {
		delete[] buffer;
		bufferSize = 0;
	}
};