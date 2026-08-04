#include "MAP.h"
#include "ds_assert_boundary.h"
struct objOBJ; // fwd (pointer element only)

// ds::impl::MAP_CONST_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>::key() @ 0x82680F60
// STRONG_ASSERT(item != 0) (d:\...\ds_map.hpp:158), then the matched node's key.
template<>
objOBJ *const &ds::impl::MAP_CONST_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::key() const
{
    typedef ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::ITEM ITEM;
    ITEM *matchedItem = reinterpret_cast<ITEM *>(this->item);
    if (!IGNORE_STRONG_ASSERT) {
        if (!matchedItem)
            ((STRONG_ASSERT_DUMMY *)nullptr)->Crash("item != 0", "d:\\projects\\code\\common\\incl.sys\\ds\\ds_map.hpp", 158, dsStrongAssertMessage);
        matchedItem = reinterpret_cast<ITEM *>(this->item);
    }
    return matchedItem->key;
}
