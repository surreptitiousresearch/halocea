#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_boundary.h"
#include "../../headers/ws/ai/aiBRAIN_frontier.h"
#include "../../headers/ws/nav/navSYS.h"
#include <string.h>

// m3d helpers (boundary). M3D_EPSILON_2 is the squared-length epsilon; m3dLengthVector_2 returns
// squared length.
extern float m3dLengthVector_2(const m3dV *v);
extern const float M3D_EPSILON_2;

// aiPLANNER::LocalizeToNavSystem(aiBRAIN*) — 0x831678C0
// Decide which nav-system(s) claim this unit's position and bind it to the closest one. When the
// unit's gravity is non-degenerate the candidate set is additionally filtered by "accepts this
// up-direction". If exactly one/several claim it, SelectNavSysIdx picks the closest; if none do,
// the unit is left without a nav-system (logged) so it does not free-lance.
void aiPLANNER::LocalizeToNavSystem(aiBRAIN *pBrain)
{
    aiScopedTimer _timer("process_frame/ai/brain/gs/init/localize");

    if (!pBrain || !aiBRAIN_ShouldAttemptLocalize(pBrain))
        return;

    m3dV pos;
    aiBRAIN_GetPos(pBrain, &pos);

    // up = -gravity; only used as a filter when it is non-degenerate.
    const m3dV *gravity = GetGravity(&pos);
    m3dV up;
    up.x = gravity->x * -1.0f;
    up.y = gravity->y * -1.0f;
    up.z = gravity->z * -1.0f;
    bool filterByUp = m3dLengthVector_2(&up) > (double)M3D_EPSILON_2;

    dsVECTOR<navSYS *, 8> accepted;
    memset(&accepted, 0, 12); // pData/nElem/allocated = 0 (matches the zero-init in the disasm)

    int nsCount = navManager->GetNavSysCount();
    for (short i = 0; i < nsCount; ++i)
    {
        navSYS *ns = navManager->GetNavSys(i);
        if (ns && ns->IsAABBBelongPoint(&pos) && (!filterByUp || ns->IsAcceptDirUp(&up)))
            accepted.PushBack(ns);
    }

    void *goal = aiBRAIN_NavGoal(pBrain);
    if (accepted.nElem)
    {
        int idx = SelectNavSysIdx(accepted, pos, aiBRAIN_DbgName(pBrain));
        aiNavGoal_SetNavSystem(goal, accepted[idx], true);
    }
    else
    {
        _apLog("~AI,Error~%s: Attention! No nav-system, don't free-lance this unit.",
               aiBRAIN_DbgName(pBrain));
        aiNavGoal_SetNavSystem(goal, nullptr, true);
    }

    dlFree(accepted.pData);
}
