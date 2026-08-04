#include "../../headers/ws/ai/aiPHYS_SMR.h"
#include "../../headers/ws/ai/aiPLANNER.h"   // aiPlanner global + GetNavManager
extern aiPLANNER *aiPlanner;   // ai08 global planner singleton
#include "../../headers/ws/nav/navMANAGER.h" // Unblock/DisconnectInstWPs + navGPH::navBLOCK_OBB
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/m3d/m3dOBB.h"
#include "../../headers/apCL.h"
#include <string.h>

extern void dlFree(void *ptr); // ws-engine allocator free (C++ linkage, per aiBRAIN_boundary.h)

// aiPHYS_SMR::ProcessRemoveObjVsNav @ 0x83238D68
// ?ProcessRemoveObjVsNav@aiPHYS_SMR@@QAAXPAVanimINST@@ABV?$dsTSTRING@D@@KG@Z
//
// Inverse of ProcessAddObjVsNav: disconnect the object's own waypoints, then — if the named
// sub-object has an OBB — unblock the nav volumes it occupied (blocker carries no owner here).
void aiPHYS_SMR::ProcessRemoveObjVsNav(animINST *pInst, const dsTSTRING<char> &objName,
                                       unsigned int flagsBlockedWP, unsigned short flagsDisabledEdge)
{
    if (!IsAcceptProcessing(pInst))
        return;

    aiPlanner->GetNavManager()->DisconnectInstWPs(pInst);

    m3dOBB obb;
    if (!aiPHYS_SMR::GetOBB(pInst, obb, objName))
        return;

    apCL cl;
    cl.line = 143;
    cl.file = "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_plan_phys_smr.cpp";

    dsVECTOR<navGPH::navBLOCK_OBB, 8> blockList;
    memset(&blockList, 0, 12);
    blockList.__cl = cl;

    navGPH::navBLOCK_OBB blocker;
    memcpy(&blocker, &obb, sizeof(m3dOBB));
    blocker.pInstOwner = nullptr;
    blockList.PushBack(blocker);

    aiPlanner->GetNavManager()->UnblockByVolList(blockList, flagsBlockedWP, flagsDisabledEdge);

    dlFree(blockList.pData);
}
