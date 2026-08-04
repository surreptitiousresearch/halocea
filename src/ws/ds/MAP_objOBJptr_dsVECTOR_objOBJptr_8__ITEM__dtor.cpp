#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::ITEM::~ITEM
// @ 0x82684448
// `key` is a bare pointer and `next` is unowned; only `value`'s backing store needs freeing.
template<>
ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::ITEM::~ITEM()
{
    dlFree(this->value.pData);
}
