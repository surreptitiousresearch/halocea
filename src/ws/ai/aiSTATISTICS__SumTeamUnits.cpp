#include "../../headers/ws/ai/aiSTATISTICS.h"

// aiSTATISTICS::SumTeamUnits() const @ 0x832AA158
// ?SumTeamUnits@aiSTATISTICS@@IBAHXZ
//
// Sum the recorded unit count across every team entry in the map. The const method mutates
// nothing, so casting away const to walk it (as the binary does via the non-const Begin) is
// faithful.
int aiSTATISTICS::SumTeamUnits() const
{
    aiSTATISTICS_TEAM_MAP *teamMap = &const_cast<aiSTATISTICS *>(this)->cntTeamUnits;

    int sum = 0;
    ds::impl::MAP_ITERATOR<dsSTRID, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> it =
        teamMap->Begin();
    while (!it.IsDone())
    {
        sum += *it.value();
        ++it;
    }
    return sum;
}
