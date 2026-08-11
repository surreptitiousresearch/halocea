#include "MAP.h"
#include "../dbg/DBG_CINE_ACTOR_INFO.h"
#include "ds_assert_boundary.h"
#include "dsSTRID.h"

// ds::impl::MAP_CONST_ITERATOR<dsSTRID,DBG_CINE_ACTOR_INFO,...>::key() @ 0x823B33A8
// STRONG_ASSERT(item != 0) (d:\projects\code\common\incl.sys\ds\ds_map.hpp:158), then the matched
// node's key.
template<>
const dsSTRID &ds::impl::MAP_CONST_ITERATOR<dsSTRID, DBG_CINE_ACTOR_INFO, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::key() const
{
    typedef ds::MAP<dsSTRID, DBG_CINE_ACTOR_INFO, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::ITEM ITEM;
    ITEM *matchedItem = reinterpret_cast<ITEM *>(this->item);
    if (!IGNORE_STRONG_ASSERT) {
        if (!matchedItem)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("item != 0", "d:\\projects\\code\\common\\incl.sys\\ds\\ds_map.hpp", 158, empty_string);
        matchedItem = reinterpret_cast<ITEM *>(this->item);
    }
    return matchedItem->key;
}
