#include "../../headers/ws/ai/aiSTATISTICS.h"
#include "../../headers/ws/ai/aiBRAIN.h"       // brain->names.nameInst
#include "../../headers/ws/gs/gsTEAMS.h"
#include "../../headers/ws/ds/dsSTRID.h"

// _apLog — engine formatted log sink. boundary.
extern void _apLog(const char *fmt, ...);

// aiSTATISTICS::NoticeTeamChanged(const aiBRAIN*, const dsSTRID&, const dsSTRID&) @ 0x832AA298
// ?NoticeTeamChanged@aiSTATISTICS@@IAAXPBVaiBRAIN@@ABVdsSTRID@@1@Z
//
// A unit moved from `oldTeam` to `newTeam`: decrement the old team's live-unit count (clamped >= 0)
// and increment the new team's, creating the map entry if absent. Each valid team that is not
// registered with gsTEAMS emits a diagnostic.
void aiSTATISTICS::NoticeTeamChanged(const aiBRAIN *brain, const dsSTRID &oldTeam,
                                    const dsSTRID &newTeam)
{
    aiSTATISTICS_TEAM_MAP *teamMap = &cntTeamUnits;

    if (oldTeam.IsValid())
    {
        if (!gsTEAMS::Instance().IsTeamRegistered(oldTeam))
            _apLog("~AI,Teams,Error~'%s': changing team from '%s', to '%s', but '%s' is not registered as team",
                   brain->names.nameInst, oldTeam.CStr(), newTeam.CStr(), oldTeam.CStr());

        int defaultCount = 1;
        ds::impl::MAP_ITERATOR<dsSTRID, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> it =
            teamMap->FindInsert(&oldTeam, &defaultCount);
        int *pCount = it.value();
        if (--*pCount < 0)
            *pCount = 0;
    }

    if (newTeam.IsValid())
    {
        if (!gsTEAMS::Instance().IsTeamRegistered(newTeam))
            _apLog("~AI,Teams,Error~'%s': changing team from '%s', to '%s', but '%s' is not registered as team",
                   brain->names.nameInst, oldTeam.CStr(), newTeam.CStr(), newTeam.CStr());

        int defaultCount = 0;
        ds::impl::MAP_ITERATOR<dsSTRID, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> it =
            teamMap->FindInsert(&newTeam, &defaultCount);
        ++*it.value();
    }
}
