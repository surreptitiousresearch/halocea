#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// boundary — ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::DUMMY,
// a debug cinematic-actor hash-map node record. Opaque here; the element is a pointer.
struct dsCineActorMapDUMMY;

// dsVECTOR<...cine-actor-map DUMMY*,8>::Back @ 0x823B5E70
// Reference to the last live element pointer (index nElem-1); assert non-empty first.
template<>
dsCineActorMapDUMMY *&dsVECTOR<dsCineActorMapDUMMY *, 8>::Back()
{
    if (!IGNORE_STRONG_ASSERT && this->nElem <= 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "nElem > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            527,
            "nElem",
            this->nElem);
    return this->pData[this->nElem - 1];
}
