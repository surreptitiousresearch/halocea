#include "../../headers/ws/ai/aiCOORDINATOR.h"

// aiCOORDINATOR::GetResultWP @ 0x83235DA8
// ?GetResultWP@aiCOORDINATOR@@UBAFPAVaiBRAIN@@@Z
//
// The waypoint id currently assigned to `pBrain` in the used-WP result map, or -1 if none.
short aiCOORDINATOR::GetResultWP(aiBRAIN *pBrain) const
{
    aiBRAIN *key = pBrain;
    ds::impl::MAP_CONST_ITERATOR<aiBRAIN *, short, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> it =
        resultsUsed.Find(&key);
    if (it.item)
        return it.value();
    return -1;
}
