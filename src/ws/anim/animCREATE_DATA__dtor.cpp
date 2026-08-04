#include "animCREATE_DATA.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free
void operator delete(void *ptr); // boundary — CRT operator delete

// animCREATE_DATA::~animCREATE_DATA() @ 0x8252CEA0 (virtual)
// Free any attached shader-replacement data (its lamp-array storage, then the record itself),
// release the affixes buffer reference, re-stamp the vtable (virtual dtor convention), then
// destroy ps.
animCREATE_DATA::~animCREATE_DATA()
{
    rendSR_DATA *srData = this->srData;
    this->__vftable = (animCREATE_DATA_vtbl *)&animCREATE_DATA::vftable;
    if (srData)
    {
        dlFree(srData->lampsData.pData);
        operator delete(srData);
    }

    dsTSTRING_BUF_HEADER<char> *affixBuffer = this->affixes.str.pBuffer;
    if (affixBuffer->refCount-- == 1)
        dlFree(affixBuffer);

    this->ps.~psSECTION();
}
