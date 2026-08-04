#include "dsTSTRING.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsTSTRING<char>::ReleaseBuffer @ 0x823B00B8
// Drop a reference on a buffer header; free it once the last reference is gone.
template<>
void dsTSTRING<char>::ReleaseBuffer(dsTSTRING_BUF_HEADER<char> *pBuf)
{
    if (pBuf->refCount-- == 1)
        dlFree(pBuf);
}
