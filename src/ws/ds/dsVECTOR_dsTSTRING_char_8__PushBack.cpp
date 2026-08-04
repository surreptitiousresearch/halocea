#include "dsVECTOR.h"
#include "dsTSTRING.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsTSTRING<char>,8>::PushBack @ 0x823B72D8
// Append a copy of `val`. A dsTSTRING is a single shared, ref-counted buffer pointer, so the copy
// just adopts val's buffer and bumps its refcount. When there is spare capacity the copy is made
// directly into the next slot; otherwise val's buffer is retained into a temporary (val may alias
// the storage being reallocated), the backing store is grown, the buffer is installed into the new
// tail slot, and the temporary reference is released. Returns the new last slot.
template<>
dsTSTRING<char> &dsVECTOR<dsTSTRING<char>, 8>::PushBack(const dsTSTRING<char> &val)
{
    int nElem = this->nElem;
    if (this->allocated >= nElem + 1) {
        dsTSTRING<char> *slot = &this->pData[nElem];
        if (slot) {
            slot->pBuffer = nullptr;
            slot->pBuffer = val.pBuffer;
            ++slot->pBuffer->refCount;
        }
    } else {
        dsTSTRING_BUF_HEADER<char> *savedBuffer = val.pBuffer;
        ++val.pBuffer->refCount;

        int allocated = this->allocated;
        int newCapacity = this->nElem + 1;
        if (2 * allocated > newCapacity)
            newCapacity = 2 * allocated;
        if (newCapacity > allocated)
            this->Realloc(newCapacity);

        if (&this->pData[this->nElem]) {
            this->pData[this->nElem].pBuffer = savedBuffer;
            ++savedBuffer->refCount;
        }
        if (savedBuffer->refCount-- == 1) // drop the temporary reference
            dlFree(savedBuffer);
    }
    ++this->nElem;
    return this->Back();
}
