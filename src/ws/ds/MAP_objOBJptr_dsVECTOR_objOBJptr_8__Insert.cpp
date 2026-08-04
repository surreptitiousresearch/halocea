#include <new> // placement new
#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Insert(ITEM**,K&,V&)
// @ 0x8268B6A8
// Private bucket-splice helper: the caller (public Insert) has already located the bucket chain
// head `*item`; this allocates one ITEM from the node pool, in-place constructs it with (key,
// *value) via the ITEM(key,value) ctor, and PREPENDS it onto the chain. On allocator exhaustion it
// writes through a null pointer rather than checking failure — reproduced faithfully (matches the
// same unconditional-crash idiom confirmed in the dsSTRID/DBG_CINE_ACTOR_INFO sibling instantiation
// and the disassembly here).
template<>
void ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Insert(
    ITEM **item, objOBJ *const &key, const dsVECTOR<objOBJ *, 8> &value)
{
    ITEM *newItem = reinterpret_cast<ITEM *>(this->allocator.Alloc());

    if (newItem) {
        new (newItem) ITEM(&key, &value);
        newItem->next = *item;
        *item = newItem;
    } else {
        // DEVIATION: decompile shows an unconditional `MEMORY[0] = *item;` on the OOM path — a
        // deliberate null-deref crash, not a decompiler artifact (matches the dsSTRID-map sibling's
        // confirmed disassembly for the same idiom).
        *reinterpret_cast<ITEM **>(0) = *item;
        *item = nullptr;
    }
}
