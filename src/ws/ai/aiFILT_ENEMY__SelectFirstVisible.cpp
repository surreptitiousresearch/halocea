#include "../../headers/ws/ai/aiFILT_ENEMY.h"
#include "../../headers/ws/ai/aiFIND_ENEMY_CDT.h"
#include "../../headers/ws/ai/aiPLANNER.h"

extern aiPLANNER *aiPlanner; // ai08 planner singleton

// aiFILT_ENEMY::SelectFirstVisible @ 0x83221120
// ?SelectFirstVisible@aiFILT_ENEMY@@QAA?AW4SELECT_RESULT@1@PAVaiBRAIN@@AAPAVentENTITY@@@Z
//
// If the default CDT system already has find-enemy results for `viewer`, hand back the visible
// candidate (or null) and report FE_RES_DONE. Otherwise (re)post the multi-candidate request and
// report FE_RES_WAIT so the caller retries next frame.
//
// NOTE: the decompiler dropped the trailing `viewer` argument on the HaveFindEnemyResults virtual
// call (same pattern as PostMultipleRequest); restored here per the (const aiBRAIN*) signature.
aiFILT_ENEMY::SELECT_RESULT aiFILT_ENEMY::SelectFirstVisible(aiBRAIN *viewer, entENTITY *&out)
{
    aiDEF_CDT_SYS *cdt = (&aiPlanner->defCDT);

    if (cdt->HaveFindEnemyResults(viewer))
    {
        aiFE_REQ_RES res;
        cdt->GetFindEnemyResults(viewer, res);
        out = res.candVisible.pHandle ? res.candVisible.pHandle->pPtr : nullptr;
        return FE_RES_DONE;
    }

    out = nullptr;
    PostMultipleRequest(viewer);
    return FE_RES_WAIT;
}
