#include "MAP.h"
#include "../dbg/DBG_CINE_ACTOR_INFO.h"
#include "dsSTRID.h"

// ds::impl::MAP_CONST_ITERATOR<dsSTRID,DBG_CINE_ACTOR_INFO,...>::IsDone() @ 0x823B2DF0
template<>
bool ds::impl::MAP_CONST_ITERATOR<dsSTRID, DBG_CINE_ACTOR_INFO, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::IsDone() const
{
    return this->item == nullptr;
}
