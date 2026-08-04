#include "dsVECTOR.h"
#include "dsTSTRING.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsTSTRING<char>,8>::Clear @ 0x823B4410
// Drop a reference on every live string buffer (freeing at zero), then reset the count.
template<>
void dsVECTOR<dsTSTRING<char>, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index) {
        dsTSTRING_BUF_HEADER<char> *buffer = this->pData[index].pBuffer;
        if (buffer->refCount-- == 1)
            dlFree(buffer);
    }
    this->nElem = 0;
}
