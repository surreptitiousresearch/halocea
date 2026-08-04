#include "MAP.h"
#include "../ps/psSECTION.h"
#include "dsTSTRING_BUF_HEADER.h"
#include "dsTSTRING.h"
#include <new> // placement new

// ds::MAP<dsTSTRING<char>,psSECTION,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Insert(ITEM**,K&,V&)
// @ 0x82712430
// Private bucket-splice helper (see MAP_dsSTRID_DBG_CINE_ACTOR_INFO__Insert.cpp for the shared
// shape). DEVIATION: the decompile aliases the freshly-allocated node as a `psSECTION *` and
// indexes it as `Empty`, `Empty[1]`, `Empty+2` (each psSECTION slot is exactly 4 bytes, so those
// strides land on ITEM::next@0, ITEM::key.pBuffer@4, ITEM::value@8) instead of the real ITEM type
// — rewritten here through the typed ITEM fields, and the raw `psSECTION::psSECTION(Empty+2,
// value)` call as the value field's copy-construction (bumps the section's refcount).
template<>
void ds::MAP<dsTSTRING<char>, psSECTION, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Insert(
    ITEM **item, const dsTSTRING<char> &key, const psSECTION &value)
{
    ITEM *newItem = reinterpret_cast<ITEM *>(this->allocator.GetEmpty());

    if (newItem) {
        newItem->next = nullptr;
        newItem->key.pBuffer = nullptr;
        dsTSTRING_BUF_HEADER<char> *keyBuffer = key.pBuffer;
        newItem->key.pBuffer = keyBuffer;
        ++keyBuffer->refCount;
        new (&newItem->value) psSECTION(value); // psSECTION::psSECTION(psSECTION const&) — refcounted copy

        newItem->next = *item;
        *item = newItem;
    } else {
        // DEVIATION: unconditional null-deref store on the OOM path, reproduced faithfully — see
        // MAP_dsSTRID_DBG_CINE_ACTOR_INFO__Insert.cpp for the same pattern.
        *reinterpret_cast<ITEM **>(0) = *item;
        *item = nullptr;
    }
}
