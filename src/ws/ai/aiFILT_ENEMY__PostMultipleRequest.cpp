#include "../../headers/ws/ai/aiFILT_ENEMY.h"
#include "../../headers/ws/ai/aiFIND_ENEMY_CDT.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiVISION_BASE.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

extern aiPLANNER *aiPlanner;      // ai08 planner singleton (aiCON_CB_globals.h / aiEXIFACE.h)
extern const m3dV m3dVZero;       // ?m3dVZero@@3Um3dV@@A
extern void dlFree(void *ptr);  // ap allocator free

// aiFILT_ENEMY::PostMultipleRequest @ 0x83220F50
// ?PostMultipleRequest@aiFILT_ENEMY@@AAAXPAVaiBRAIN@@@Z
//
// Build one find-enemy visibility request covering up to the first 10 candidates and post it to the
// planner's default CDT system. Each candidate contributes an enCANDIDATE holding its entity plus a
// filled aiVIS_REQ_PARAMS (eye + two vis-check sample points + body dir-up).
//
// NOTE: the decompiler dropped the trailing `viewer` argument on the CanPostFindEnemyRequest /
// (later) HaveFindEnemyResults virtual calls; disassembly (0x83220F6C-7C: r3=&defCDT, r4=viewer)
// confirms `viewer` is passed. Restored here.
void aiFILT_ENEMY::PostMultipleRequest(aiBRAIN *viewer)
{
    aiDEF_CDT_SYS *cdt = (&aiPlanner->defCDT);
    if (!cdt->CanPostFindEnemyRequest(viewer))
        return;

    aiFE_REQ_PARAMS req;

    m3dV posEye;
    viewer->mind.vision.pointee->CalcPosEye(&posEye);

    int nElem = arr.nElem;
    if (nElem >= 10)
        nElem = 10;

    for (int i = 0; i < nElem; ++i)
    {
        aiWATCHER *watcher = arr[i].hold->watcher;

        m3dV visA = m3dVZero;
        m3dV visB = m3dVZero;
        GetVisCheckParams(viewer, watcher, &visA, &visB);
        _m3dCheckValid(&visA);
        _m3dCheckValid(&visB);

        m3dV posCheck;
        posCheck.x = visA.x + visB.x;
        posCheck.y = visA.y + visB.y;
        posCheck.z = visA.z + visB.z;

        m3dV dirUp;
        watcher->GetBodyDirUp(&dirUp);

        aiFE_REQ_PARAMS::enCANDIDATE cand;
        cand.ent = watcher->GetEntity();
        cand.params.FillParams(viewer, posEye, m3dVZero, posCheck, m3dVZero, dirUp);
        req.candidates.PushBack(cand);
    }

    if (req.candidates.nElem)
    {
        req.pSender = viewer;
        cdt->PostFindEnemyRequest(req);
    }

    dlFree(req.candidates.pData);
}
