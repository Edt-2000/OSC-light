#pragma once

class Print {
public:
	char * buffer;
	char * fullBuffer;
	int bufferSize = 0;

	void write(const char * data, int dataCount) {
		if (bufferSize > 0 && dataCount > bufferSize) {
			delete buffer;
			delete fullBuffer;
		}

		buffer = new char[dataCount];
		fullBuffer = new char[dataCount];

		bufferSize = dataCount;

		memcpy_s(buffer, dataCount, data, dataCount);
		memcpy_s(fullBuffer, dataCount, data, dataCount);

		for (int i = 0; i < dataCount - 1; i++) {
			if (fullBuffer[i] == '\0') {
				fullBuffer[i] = '_';
			}
		}
		fullBuffer[dataCount] = '\0';
	}
};