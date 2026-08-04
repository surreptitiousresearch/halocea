#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/gs/gsTEAMS.h"

// The ai08 planner singleton (aiEXIFACE.h). Declared here to avoid pulling that header's
// conflicting gsTEAMS boundary definition into this TU.
extern aiPLANNER *aiPlanner;

// m3d subsystem — squared distance between two points. boundary.
extern float m3dDist_2(const m3dV *a, const m3dV *b);

// aiTEAMS_SYS::CountMyNeutralsNearPoint(const propGAME_INFO_TEAM&, const m3dV&, float) const
//   @ 0x83173520  ?CountMyNeutralsNearPoint@aiTEAMS_SYS@@QBAHABVpropGAME_INFO_TEAM@@ABUm3dV@@M@Z
// Count active AI units within `radius` of `pos` whose team is rated neutral toward `teamMy`.
int aiTEAMS_SYS::CountMyNeutralsNearPoint(const propGAME_INFO_TEAM &teamMy, const m3dV &pos,
                                         float radius) const
{
    float radiusSq = radius * radius;
    int count = 0;

    for (int i = 0; i < aiPlanner->wah.arrUnits.nElem; ++i)
    {
        aiWATCHER_BRAIN *unit = aiPlanner->wah.PeekActive(i);
        if (!unit)
            continue;

        m3dV bodyPos;
        unit->GetBodyPos(&bodyPos);
        if (m3dDist_2(&pos, &bodyPos) > radiusSq)
            continue;

        propGAME_INFO_TEAM unitTeam;              // ctor installs the team vtable
        unitTeam.team = unit->GetTeamID()->team;
        unitTeam.EnsureValid();

        dsSTRID mine;
        mine.id = teamMy.team.id;
        dsSTRID theirs;
        theirs.id = unitTeam.team.id;
        if (gsTEAMS::Instance().GetAttitude(mine, theirs) == gsTEAMS::ATT_NEUTRAL)
            ++count;
    }

    return count;
}
