#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::ITEM::ITEM
// @ 0x8268A288
// next=null, key=*key, value default-constructed then bulk-populated from `*value`'s elements via
// dsVECTOR::Insert(0, pData, nElem) — reproduces the disassembly's exact call, not a plain copy.
template<>
ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::ITEM::ITEM(
    objOBJ *const *key, const dsVECTOR<objOBJ *, 8> *value)
    : next(nullptr), key(*key)
{
    // DEVIATION: not a default-constructed `value` — the decompile explicitly copies the source
    // vector's call-site cookie (`value->__cl`) before bulk-inserting, so the new node's storage is
    // tagged with the caller's allocation site, not this ctor's.
    this->value.pData = nullptr;
    this->value.nElem = 0;
    this->value.allocated = 0;
    this->value.__cl = value->__cl;
    this->value.Insert(0, value->pData, value->nElem);
}
