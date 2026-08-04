// apCONFIG::GetRawValue @0x82623188 — resolve `complexKey`: if it's registered in `exportMap`
// (and gStateMgr is live), return a copy of the tracked dsSTATE_MGR state directly; otherwise
// fall back to a plain psSECTION lookup on `ps`.
#include "../../headers/ws/ap/apCONFIG.h"
#include "../../headers/ws/ds/dsSTATE_MGR.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"

dsDATA apCONFIG::GetRawValue(const char *complexKey) const
{
    dsSTRID key(complexKey, true);
    if (gStateMgr && key.IsValid()) {
        ds::impl::MAP_ITERATOR<dsSTRID, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> it =
            const_cast<ds::MAP<dsSTRID, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> &>(exportMap).Find(&key);
        if (it.item) {
            dsDATA result;
            result.type = nullptr;
            const dsDATA *state = gStateMgr->GetState(*it.value());
            result.StoreValue(*state);
            return result;
        }
    }

    // DEVIATION: the raw decompile invokes GetValue through `this` cast directly to
    // psSECTION_INTERFACE* (not through the `ps` member) — apCONFIG's own vtable-having base
    // aliases psSECTION_INTERFACE's layout at offset 0. Modeled here as the equivalent call on
    // `ps` (the psSECTION this config wraps), which is the operation's actual intent.
    return reinterpret_cast<const psSECTION_INTERFACE &>(ps).GetValue(complexKey, 0);
}
