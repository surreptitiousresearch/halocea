#include "dsVECTOR.h"

// boundary — ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::DUMMY,
// a debug cinematic-actor hash-map node record. Opaque here; the element is a pointer.
struct dsCineActorMapDUMMY;

// dsVECTOR<dsCineActorMapDUMMY *,8>::PushBack @ 0x823B77F8
// Append `val` to the tail, growing the backing store first when there is no spare capacity,
// and return a reference to the newly stored pointer slot.
template<>
dsCineActorMapDUMMY *&dsVECTOR<dsCineActorMapDUMMY *, 8>::PushBack(dsCineActorMapDUMMY *const &val)
{
    if (this->allocated >= this->nElem + 1) {
        // spare capacity: store straight into the tail slot
        this->pData[this->nElem] = val;
    } else {
        // must grow: save `val` first in case it aliases storage about to be reallocated
        dsCineActorMapDUMMY *saved = val;
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        this->pData[this->nElem] = saved;
    }
    ++this->nElem;
    return this->Back();
}
