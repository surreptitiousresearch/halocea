#include "dsTSTRING.h"

// Debug global allocator the ws build links in: dlMalloc(size, file, line).
extern "C" void *dlMalloc(unsigned int size, const char *file, unsigned int line); // boundary

// dsTSTRING<char>::AllocBuffer @ 0x823B0240
// Allocate a fresh buffer header able to hold `len` chars (capacity at least `maxLen`),
// NUL-terminated, with refCount 1.
template<>
dsTSTRING_BUF_HEADER<char> *dsTSTRING<char>::AllocBuffer(int maxLen, int len)
{
    int capacity = maxLen;
    if (capacity < len)
        capacity = len;
    dsTSTRING_BUF_HEADER<char> *buffer = (dsTSTRING_BUF_HEADER<char> *)dlMalloc(
        capacity + 16, "d:\\projects\\code\\common\\incl.sys\\ds\\ds_string.hpp", 0x3B0u);
    buffer->strLen = len;
    buffer->refCount = 1;
    buffer->maxStrLen = capacity;
    buffer->str[len] = 0;
    return buffer;
}
