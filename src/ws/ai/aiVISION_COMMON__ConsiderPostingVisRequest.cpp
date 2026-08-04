#include "../../headers/ws/ai/aiVISION_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h" // st.pointee->enemy.visQuery
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiVISION_COMMON::ConsiderPostingVisRequest @ 0x832912F0
// ?ConsiderPostingVisRequest@aiVISION_COMMON@@MAAXXZ
// Protected virtual: decide what to do with a pending enemy-visibility probe. When no real query is
// pending, do nothing. When the arm must stay invisible, store a zero result. Otherwise recompute
// the eye position, run the cheap potential-visibility test against the random sight point, and
// either store an instant result (Monte-Carlo hit), post a full line-of-sight request, or store a
// zero result.
void aiVISION_COMMON::ConsiderPostingVisRequest()
{
    aiSTATUS *st = pBrain->st.pointee;
    if (!st->enemy.visQuery.isReal)
        return;

    if (ShouldKeepInvisible())
    {
        StoreLatestTest(0.0f);
        return;
    }

    m3dV posCheck = st->enemy.visQuery.points.posRandom;

    m3dV eye;
    CalcPosEye(&eye);
    posEye = eye;

    if (IsPotentiallyVisible(posEye, posCheck))
    {
        if (IsVisibleByMonteCarlo())
            StoreLatestTest(100.0f);
        else
            PostVisRequest();
    }
    else
    {
        StoreLatestTest(0.0f);
    }
}
