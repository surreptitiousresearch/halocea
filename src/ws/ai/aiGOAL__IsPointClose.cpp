// aiGOAL::IsPointClose @ 0x832AF520
// ?IsPointClose@aiGOAL@@UAA_NABUm3dV@@_N@Z
#include "../../headers/ws/ai/aiGOAL.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// True when the body is within the command reach distance of `goal`. With 2-D navigation the reach
// test uses horizontal (XZ) distance plus a 5-unit vertical gate; otherwise a straight 3-D distance.
bool aiGOAL::IsPointClose(const m3dV &goal, bool use2DNavigation)
{
    m3dV pos = pBrain->GetPos();
    float distReach = pBrain->cmd.goal.distReach;
    if (!use2DNavigation)
        return m3dDist(&pos, &goal) < distReach;

    aiSTATUS *st = pBrain->st.pointee;
    pBrain->LocalHeightAndDistXZ(pos, goal, st->goal.heightDiff, st->goal.distXZ);
    if (st->goal.distXZ >= distReach)
        return false;
    if (st->goal.heightDiff >= 5.0f)
        return false;
    return true;
}
