#include "dsTSTRING.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsTSTRING<char>::~dsTSTRING<char> @ 0x823B0100
// Drop this string's reference on its shared buffer header, freeing it when the count hits zero.
template<>
dsTSTRING<char>::~dsTSTRING()
{
    dsTSTRING_BUF_HEADER<char> *buffer = this->pBuffer;
    if (buffer->refCount-- == 1)
        dlFree(buffer);
}
