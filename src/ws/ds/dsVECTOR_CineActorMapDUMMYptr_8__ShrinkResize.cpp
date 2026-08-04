#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// boundary — ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::DUMMY,
// a debug cinematic-actor hash-map node record. Opaque here; the element is a pointer.
struct dsCineActorMapDUMMY;

// dsVECTOR<...cine-actor-map DUMMY*,8>::ShrinkResize @ 0x823B5B48
// Shrink the live count to `size`. The element is a raw pointer (trivially destructible),
// so the generic per-element destructor loop is empty and collapses to resetting the count.
template<>
void dsVECTOR<dsCineActorMapDUMMY *, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size",
            size);
    if (size < this->nElem)
        this->nElem = size;
}
