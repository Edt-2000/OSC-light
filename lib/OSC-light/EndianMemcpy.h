#pragma once

#include <stddef.h>
#include <cstring>

inline void memcpy(char *target, const char *source, size_t size, const bool platformIsBigEndian)
{
    if (platformIsBigEndian)
    {
        memcpy(target, source, size);
    }
    else
    {
        for (size_t d = 0; d < size; ++d)
        {
            target[d] = source[size - 1 - d];
        }
    }
}