#include "../../headers/ws/ai/aiSTATISTICS.h"
#include "../../headers/ws/ds/dsSTRID.h"

// aiSTATISTICS::GetActiveUnitsCnt(dsSTRID) @ 0x832A9F98
// ?GetActiveUnitsCnt@aiSTATISTICS@@QAAHVdsSTRID@@@Z
//
// Non-const / by-value overload: the recorded live-unit count for team `team` (0 when the team has
// no entry in the map).
int aiSTATISTICS::GetActiveUnitsCnt(dsSTRID team)
{
    aiSTATISTICS_TEAM_MAP *teamMap = &cntTeamUnits;

    ds::impl::MAP_ITERATOR<dsSTRID, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> it =
        teamMap->Find(&team);
    if (it.item)
        return *it;
    return 0;
}
