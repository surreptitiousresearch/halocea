#include "MAP.h"
#include "../ps/psSECTION.h"
#include "ds_assert_boundary.h"

// ds::impl::MAP_CONST_ITERATOR<dsTSTRING<char>,psSECTION,...>::key() @ 0x8270F3D0
// STRONG_ASSERT(item != 0) (ds_map.hpp:158), then the matched node's key.
template<>
const dsTSTRING<char> &ds::impl::MAP_CONST_ITERATOR<dsTSTRING<char>, psSECTION, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::key() const
{
    typedef ds::MAP<dsTSTRING<char>, psSECTION, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::ITEM ITEM;
    ITEM *matchedItem = reinterpret_cast<ITEM *>(this->item);
    if (!IGNORE_STRONG_ASSERT) {
        if (!matchedItem)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("item != 0", "d:\\projects\\code\\common\\incl.sys\\ds\\ds_map.hpp", 158, empty_string);
        matchedItem = reinterpret_cast<ITEM *>(this->item);
    }
    return matchedItem->key;
}
