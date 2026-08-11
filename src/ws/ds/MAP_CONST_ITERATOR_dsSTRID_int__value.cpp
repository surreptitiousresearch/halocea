#include "MAP.h"
#include "ds_assert_boundary.h"
#include "dsSTRID.h"

// ds::impl::MAP_CONST_ITERATOR<dsSTRID,int,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::value()
// @ 0x82766AE8 (used by apCONFIG::GetRawValue). Same STRONG_ASSERT(item != 0) (ds_map.hpp:169)
// shape as the DBG_CINE_ACTOR_INFO instantiation's ::value() — see
// MAP_CONST_ITERATOR_dsSTRID_DBG_CINE_ACTOR_INFO__value.cpp.
template<>
const int &ds::impl::MAP_CONST_ITERATOR<dsSTRID, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::value() const
{
    typedef ds::MAP<dsSTRID, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::ITEM ITEM;
    ITEM *matchedItem = reinterpret_cast<ITEM *>(this->item);
    if (!IGNORE_STRONG_ASSERT) {
        if (!matchedItem)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("item != 0", "d:\\projects\\code\\common\\incl.sys\\ds\\ds_map.hpp", 169, empty_string);
    }
    return matchedItem->value;
}
