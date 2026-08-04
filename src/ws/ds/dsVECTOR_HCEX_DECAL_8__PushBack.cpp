#include "dsVECTOR.h"
#include "../../headers/hcex/HCEX_DECAL.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<HCEX_DECAL,8>::PushBack @ 0x823E26C0
// Append a copy of `val` to the tail (copy-constructing its ref-counted `name` string), growing
// the backing store first when there is no spare capacity, and return a reference to the new slot.
template<>
HCEX_DECAL &dsVECTOR<HCEX_DECAL, 8>::PushBack(const HCEX_DECAL &val)
{
    int nElem = this->nElem;
    if (this->allocated >= nElem + 1) {
        // spare capacity: construct straight into the tail slot
        HCEX_DECAL *slot = &this->pData[nElem];
        slot->pos = val.pos;
        slot->name.pBuffer = 0;
        dsTSTRING_BUF_HEADER<char> *buffer = val.name.pBuffer;
        slot->name.pBuffer = buffer;
        ++buffer->refCount;
    } else {
        // must grow: hold `val`'s fields (and a name reference) across the reallocation
        dsTSTRING_BUF_HEADER<char> *savedBuffer = val.name.pBuffer;
        float x = val.pos.x;
        float y = val.pos.y;
        float z = val.pos.z;
        ++savedBuffer->refCount;

        int allocated = this->allocated;
        int newCapacity = this->nElem + 1;
        if (2 * allocated > newCapacity)
            newCapacity = 2 * allocated;
        if (newCapacity > allocated)
            this->Realloc(newCapacity);

        HCEX_DECAL *slot = &this->pData[this->nElem];
        slot->pos.x = x;
        slot->pos.y = y;
        slot->pos.z = z;
        slot->name.pBuffer = savedBuffer;
        ++savedBuffer->refCount;

        // drop the transient hold taken before the realloc
        if (savedBuffer->refCount-- == 1)
            dlFree(savedBuffer);
    }
    ++this->nElem;
    return this->Back();
}
