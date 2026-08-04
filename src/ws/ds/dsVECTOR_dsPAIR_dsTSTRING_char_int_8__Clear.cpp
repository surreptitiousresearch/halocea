#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsPAIR<dsTSTRING<char>,int>,8>::Clear @ 0x823B5058
// Drop a reference on every live pair's string key (freeing at zero), then reset the count.
template<>
void dsVECTOR<dsPAIR<dsTSTRING<char>, int>, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index) {
        dsTSTRING_BUF_HEADER<char> *buffer = this->pData[index].key.pBuffer;
        if (buffer->refCount-- == 1)
            dlFree(buffer);
    }
    this->nElem = 0;
}
