#include "dsTSTRING.h"

// Debug global reallocator the ws build links in: dlRealloc(ptr, size, file, line).
extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsTSTRING<char>::ReallocBuffer @ 0x823B0CF0
// Grow/shrink an existing buffer header in place to hold `len` chars (capacity at least
// `maxLen`), re-terminate, and reset its refCount to 1.
template<>
dsTSTRING_BUF_HEADER<char> *dsTSTRING<char>::ReallocBuffer(dsTSTRING_BUF_HEADER<char> *pBuf,
                                                           int maxLen, int len)
{
    if (maxLen < len)
        maxLen = len;
    int capacity = maxLen;
    dsTSTRING_BUF_HEADER<char> *buffer = (dsTSTRING_BUF_HEADER<char> *)dlRealloc(
        pBuf, capacity + 16, "d:\\projects\\code\\common\\incl.sys\\ds\\ds_string.hpp", 0x3DEu);
    buffer->strLen = len;
    buffer->refCount = 1;
    buffer->maxStrLen = capacity;
    buffer->str[len] = 0;
    return buffer;
}
