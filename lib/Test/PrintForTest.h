#pragma once

#include <cstring>

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

class Print
{
public:
    char *buffer;
    char *fullBuffer;
    char *reversedBuffer;
    int bufferSize = 0;
    int internalPointer = 0;

    void write(const char *data, int dataCount)
    {
        if (bufferSize > 0 && dataCount > bufferSize)
        {
            delete buffer;
            delete fullBuffer;
            delete reversedBuffer;
        }

        buffer = new char[dataCount + 1];
        fullBuffer = new char[dataCount + 1];
        reversedBuffer = new char[dataCount + 1];

        bufferSize = dataCount;

        memcpy(buffer, data, dataCount);
        memcpy(fullBuffer, data, dataCount);
        memcpy(reversedBuffer, data, dataCount);

        for (int i = 0; i < dataCount; i++)
        {
            if (fullBuffer[i] == '\0')
            {
                fullBuffer[i] = '_';
            }

            if (reversedBuffer[i] == '_')
            {
                reversedBuffer[i] = '\0';
            }
        }

        fullBuffer[dataCount] = '\0';
        reversedBuffer[dataCount] = '\0';

        internalPointer = 0;
    }
};