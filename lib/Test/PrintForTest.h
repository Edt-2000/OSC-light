#pragma once

#include <cstring>

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

class Print
{
protected:
    void reserve(const int dataCount)
    {
        if (dataCount >= internalBufferSize)
        {
            delete printBuffer;
            delete printUnderscoredBuffer;
            delete printNormalizedBuffer;

            internalBufferSize = dataCount + 16;

            printBuffer = new char[internalBufferSize];
            printUnderscoredBuffer = new char[internalBufferSize];
            printNormalizedBuffer = new char[internalBufferSize];
        }
    }

public:
    char *printBuffer;
    char *printUnderscoredBuffer;
    char *printNormalizedBuffer;
    int dataCountInBuffer = 0;
    int internalBufferSize = 0;
    int internalPointer = 0;

    void write(const char *data, const int dataCount)
    {
        reserve(dataCount);

        dataCountInBuffer = dataCount;

        memcpy(printBuffer, data, dataCount);
        memcpy(printUnderscoredBuffer, data, dataCount);
        memcpy(printNormalizedBuffer, data, dataCount);

        for (int i = 0; i < dataCount; i++)
        {
            if (printUnderscoredBuffer[i] == '\0')
            {
                printUnderscoredBuffer[i] = '_';
            }

            if (printNormalizedBuffer[i] == '_')
            {
                printNormalizedBuffer[i] = '\0';
            }
        }

        printUnderscoredBuffer[dataCount] = '\0';
        printNormalizedBuffer[dataCount] = '\0';

        internalPointer = 0;
    }

    void flush()
    {
        dataCountInBuffer = 0;
    }

    void normalizeBuffer()
    {
        memcpy(printBuffer, printNormalizedBuffer, dataCountInBuffer);
    }
};