#pragma once

class Print {
public:
	char * buffer;
	char * fullBuffer;
	char * reversedBuffer;
	int bufferSize = 0;

	void write(const char * data, int dataCount) {
		if (bufferSize > 0 && dataCount > bufferSize) {
			delete buffer;
			delete fullBuffer;
			delete reversedBuffer;
		}

		buffer = new char[dataCount + 1];
		fullBuffer = new char[dataCount + 1];
		reversedBuffer = new char[dataCount + 1];

		bufferSize = dataCount;

		memcpy_s(buffer, dataCount, data, dataCount);
		memcpy_s(fullBuffer, dataCount, data, dataCount);
		memcpy_s(reversedBuffer, dataCount, data, dataCount);

		for (int i = 0; i < dataCount; i++) {
			if (fullBuffer[i] == '\0') {
				fullBuffer[i] = '_';
			}

			if (reversedBuffer[i] == '_') {
				reversedBuffer[i] = '\0';
			}
		}

		fullBuffer[dataCount] = '\0';
		reversedBuffer[dataCount] = '\0';
	}
};