#include "MAP.h"
#include "ds_assert_boundary.h"
struct objOBJ; // fwd (pointer element only)

// ds::impl::MAP_CONST_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>::operator->() @ 0x82680B30
// STRONG_ASSERT(item != 0) (D:\...\INCL.SYS\ds/ds_map.h:81), then a pointer to the matched node's
// value.
template<>
const dsVECTOR<objOBJ *, 8> *ds::impl::MAP_CONST_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::operator->() const
{
    typedef ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::ITEM ITEM;
    ITEM *matchedItem = reinterpret_cast<ITEM *>(this->item);
    if (!IGNORE_STRONG_ASSERT) {
        if (!matchedItem)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("item != 0", "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_map.h", 81, empty_string);
        matchedItem = reinterpret_cast<ITEM *>(this->item);
    }
    return &matchedItem->value;
}
