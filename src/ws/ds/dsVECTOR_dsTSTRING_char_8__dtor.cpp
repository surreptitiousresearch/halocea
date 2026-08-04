#include "dsVECTOR.h"
#include "dsTSTRING.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsTSTRING<char>,8>::~dsVECTOR @ 0x823B43A8
// Drop a reference on every live string buffer (freeing at zero), then free the backing storage.
template<>
dsVECTOR<dsTSTRING<char>, 8>::~dsVECTOR()
{
    for (int index = 0; index < this->nElem; ++index) {
        dsTSTRING_BUF_HEADER<char> *buffer = this->pData[index].pBuffer;
        if (buffer->refCount-- == 1)
            dlFree(buffer);
    }
    dlFree(this->pData);
}
