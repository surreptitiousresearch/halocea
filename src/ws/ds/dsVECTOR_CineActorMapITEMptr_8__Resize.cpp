#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// boundary — ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::ITEM,
// a debug cinematic-actor hash-map key/value entry. Opaque here; the element is a pointer.
struct dsCineActorMapITEM;

// dsVECTOR<...cine-actor-map ITEM*,8>::Resize(int, const T&) @ 0x823B8118
// Resize the live count to `size`, copy-filling any newly exposed pointer slots with `val`.
// Growing past capacity reallocates first (saving `val` in case it aliases the old storage).
// Shrinking just drops the trailing slots (pointer elements are trivially destructible).
template<>
void dsVECTOR<dsCineActorMapITEM *, 8>::Resize(int size, dsCineActorMapITEM *const &val)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            238,
            "size",
            size);

    int nElem = this->nElem;
    if (size < nElem) {
        this->nElem = size; // shrink: trivial per-slot destructor loop elided
        return;
    }
    if (size <= nElem) {
        this->nElem = size;
        return;
    }
    if (size <= this->allocated) {
        for (int index = nElem; index < size; ++index)
            this->pData[index] = val;
        this->nElem = size;
        return;
    }

    dsCineActorMapITEM *fill = val; // hold across the reallocation
    this->Realloc(size);
    for (int index = this->nElem; index < size; ++index)
        this->pData[index] = fill;
    this->nElem = size;
}
