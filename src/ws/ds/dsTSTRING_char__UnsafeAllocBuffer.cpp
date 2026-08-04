#include "dsTSTRING.h"

// Debug global allocator the ws build links in: dlMalloc(size, file, line).
extern "C" void *dlMalloc(unsigned int size, const char *file, unsigned int line); // boundary

// dsTSTRING<char>::UnsafeAllocBuffer @ 0x823B0138
// Allocate a fresh buffer header able to hold `len` chars (capacity at least `maxLen`),
// NUL-terminated, with refCount 1. Does not touch this->pBuffer.
template<>
dsTSTRING_BUF_HEADER<char> *dsTSTRING<char>::UnsafeAllocBuffer(int maxLen, int len)
{
    if (maxLen < len)
        maxLen = len;
    int capacity = maxLen;
    dsTSTRING_BUF_HEADER<char> *buffer = (dsTSTRING_BUF_HEADER<char> *)dlMalloc(
        capacity + 16, "d:\\projects\\code\\common\\incl.sys\\ds\\ds_string.hpp", 0x3B0u);
    buffer->strLen = len;
    buffer->refCount = 1;
    buffer->maxStrLen = capacity;
    buffer->str[len] = 0;
    return buffer;
}
