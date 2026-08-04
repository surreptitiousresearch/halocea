#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ai/aiSTRIKE_FILTER.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiWEAPON_BASE.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/CMP.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"

// ai08 planner singleton (?aiPlanner@@3PAVaiPLANNER@@A). boundary data.
extern aiPLANNER *aiPlanner;
// Blam allocator free. boundary.
extern "C" void dlFree(void *ptr);

// wpn subsystem — weapon strike descriptor (opaque; only pointers/refs are handled here). boundary.
struct wpnS_DESC;

// ai08 coordinated squad/group — the two group-query virtuals reached here. boundary subset.
struct aiCOORD_GROUP {
    // 0x83185... (?GetMembersCount@aiCOORD_GROUP@@UAAHXZ) — number of brains in the group.
    int GetMembersCount();
    // 0x83185... (?GetBrain@aiCOORD_GROUP@@UBAPBVaiBRAIN@@H@Z) — group member `idx`'s brain, or null.
    const aiBRAIN *GetBrain(int idx) const;
};

// aiGRASS::SmokeOutFromHO @ 0x83184E10
// A coordinated group's target has been smoked out of its hideout. Start tracking the group's
// "victim" watcher (once), then, so the group can immediately re-engage, clear the sparsing
// reservations for every (weapon-strike-descriptor, team) combination the group can bring to bear.
void aiGRASS::SmokeOutFromHO(aiCOORD_GROUP &group)
{
    aiWATCHER *victim = this->GetVictim(&group);
    if (!victim)
        return;

    // Skip if this victim is already being tracked.
    for (ds::LIST<aiWATCHER *>::DATA *n = this->victims.head; n; n = n->next)
        if (n->val == victim)
            return;

    aiWATCHER *watcher = aiWATCHER::Create(victim->GetActor());
    if (!watcher)
        return;

    this->victims.PushBack(watcher);

    aiSTRIKE_FILTER filter;
    filter.grenades = fvONLY; // consider only grenade strikes when enumerating descriptors

    dsVECTOR<dsSTRID, 8>          teamIds;  // distinct team ids across the group
    dsVECTOR<const wpnS_DESC *, 8> allDescs; // distinct grenade-strike descriptors across the group

    int members = group.GetMembersCount();
    int descCount = 0;
    int teamCount = 0;
    if (members > 0)
    {
        for (int m = 0; m < members; ++m)
        {
            const aiBRAIN *brain = group.GetBrain(m);
            if (!brain)
                continue;

            dsSTRID teamId = const_cast<aiBRAIN *>(brain)->GetTeamID()->team;
            ds::CMP cmp;
            if (teamIds.Find<ds::CMP, dsSTRID>(teamId, cmp, 0) == -1)
                teamIds.PushBack(teamId);

            dsVECTOR<const wpnS_DESC *, 8> descs;
            brain->body.pointee->wpn.pointee->GetSDescVectorByFilter(filter, descs);
            for (int i = 0; i < descs.nElem; ++i)
            {
                const wpnS_DESC *desc = descs[i];
                ds::CMP cmpDesc;
                if (allDescs.Find<ds::CMP, const wpnS_DESC *>(desc, cmpDesc, 0) < 0)
                    allDescs.PushBack(descs[i]);
            }
            dlFree(descs.pData);
        }
        descCount = allDescs.nElem;
        teamCount = teamIds.nElem;
    }

    aiGLOSS *gloss = (&aiPlanner->gloss);
    for (int j = 0; j < descCount; ++j)
    {
        const wpnS_DESC *desc = allDescs[j];
        for (int k = 0; k < teamCount; ++k)
        {
            dsSTRID team = teamIds[k];
            gloss->ClearSparsing(*desc, team);
        }
    }

    dlFree(teamIds.pData);
    dlFree(allDescs.pData);
    dlFree(filter.flags.propList.pData);
}
