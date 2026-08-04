#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // extern aiPLANNER *aiPlanner
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsTYPE_ID.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiPLANNER::implCountLiveInDomain(iaIACTOR*, const dsTSTRING<char>&) @ 0x83264080
// ?implCountLiveInDomain@aiPLANNER@@UAAHPAViaIACTOR@@ABV?$dsTSTRING@D@@@Z
//
// SSL op ($AI.CountLiveInDomain): count the live (non-dead) active unit watchers that (optionally)
// belong to team `team` AND whose body position lies inside the domain actor `dom`. `dom` must be a
// gsDOMAIN_DESC-derived actor; otherwise it logs the "wrong argument" diagnostic and returns 0.

extern void _apLog(const char *fmt, ...);

struct propGAME_INFO_TEAM; // prop — per-actor team info    boundary (only the interned team id @+4 is read)

// gsDOMAIN_DESC RTTI anchor the binary tests `dom`'s dynamic type against. boundary (static member
// declared, not defined here — the real storage lives in the gs domain subsystem).
struct gsDOMAIN_DESC { static dsTYPE_ID TYPE_ID; };

// Domain point-in-region test. The binary dispatches this through `dom`'s scnDOMAIN subobject —
// decompiled literally as `((int(*)(iaIACTOR*, _BYTE*, _DWORD))dom[1].PreProcessMsg)(&dom[1], &pos, 0)`
// (i.e. the actor sitting one iaIACTOR-stride above the passed base). Routed through a boundary
// helper rather than fabricating the scnDOMAIN layout / dispatch-slot here.
extern int aiDomainContainsPoint(iaIACTOR *dom, const m3dV *pos);

int aiPLANNER::implCountLiveInDomain(iaIACTOR *dom, const dsTSTRING<char> &team)
{
    if (dom)
    {
        dsTYPE_ID domType = dom->TypeID();
        if (domType.IsDerivedFrom(gsDOMAIN_DESC::TYPE_ID))
        {
            dsSTRID teamStrid(team, 0);
            bool filterByTeam = team.pBuffer->strLen != 0;

            int count = 0;
            int nElem = aiPlanner->wah.arrUnits.nElem;
            for (int idx = 0; idx < nElem; ++idx)
            {
                aiWATCHER_BRAIN *w = aiPlanner->wah.PeekActive(idx);
                if (!w || w->IsDead())
                    continue;

                if (filterByTeam)
                {
                    propGAME_INFO_TEAM *wTeam = w->GetTeamID();
                    // decompiler reads the interned team id at team-info offset +4 (a dsSTRID.id).
                    if (teamStrid.id != *(const char *const *)((const char *)wTeam + 4))
                        continue;
                }

                m3dV pos;
                w->GetBodyPos(&pos);
                if (aiDomainContainsPoint(dom, &pos))
                    ++count;
            }
            return count;
        }
    }

    _apLog("~AI,Error~: wrong argument for $AI.CountLiveInDomain()");
    return 0;
}
