#include "msgSYSTEM.h"
#include "msgADDR.h"
#include "../ds/MAP.h"
#include "../ds/ds_assert_boundary.h"

// The uniqueID -> msgADDR* hash the message system keeps at msgSYSTEM::addrHash (held there as an
// opaque 84-byte footprint to keep the outer layout intact).
typedef ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> MsgAddrUidMap;

// msgSYSTEM::AssignUID @ 0x8255D4F8
// Publish `pComm`'s uniqueID in the uid hash so it can be looked up by id. No-op for addresses
// with a negative (unassigned) id or ones already published (state bit 0x100). Asserts that any
// pre-existing entry for the id maps back to `pComm`.
void msgSYSTEM::AssignUID(msgADDR *pComm)
{
    if (pComm->uniqueID < 0)
        return;

    MsgAddrUidMap *uidMap = (MsgAddrUidMap *)&this->addrHash;
    int uniqueID = pComm->uniqueID;

    ds::impl::MAP_ITERATOR<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> found =
        uidMap->Find(&uniqueID);
    if (!IGNORE_STRONG_ASSERT && !found.IsDone() && *found.value() != pComm)
        STRONG_ASSERT_DUMMY().Crash(
            "it.IsDone() || it.value() == pComm",
            "D:\\Projects\\code\\common\\src.sys\\gm_sys\\msg\\msgaddr.cpp",
            835,
            empty_string);

    if ((pComm->stateAddr.state & 0x100) == 0) {
        uidMap->Insert(&uniqueID, &pComm);
        pComm->stateAddr.state |= 0x100u;
    }
}
