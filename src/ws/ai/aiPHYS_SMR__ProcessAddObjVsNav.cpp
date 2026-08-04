#include "../../headers/ws/ai/aiPHYS_SMR.h"
#include "../../headers/ws/ai/aiPLANNER.h"   // aiPlanner global + GetNavManager
extern aiPLANNER *aiPlanner;   // ai08 global planner singleton
#include "../../headers/ws/nav/navMANAGER.h" // Block/AutolinkInstWPs + navGPH::navBLOCK_OBB
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/m3d/m3dOBB.h"
#include "../../headers/apCL.h"
#include <string.h>

extern void dlFree(void *ptr); // ws-engine allocator free (C++ linkage, per aiBRAIN_boundary.h)

// aiPHYS_SMR::ProcessAddObjVsNav @ 0x83238C80
// ?ProcessAddObjVsNav@aiPHYS_SMR@@QAAXPAVanimINST@@ABV?$dsTSTRING@D@@KG@Z
//
// Register a movable object's named sub-piece against the nav graph: (re)link the object's own
// waypoints, then — if the named sub-object has an OBB — block the nav volumes it occupies using the
// caller-supplied WP-block / edge-disable flag sets. No-op when the instance isn't accepted for SMR.
void aiPHYS_SMR::ProcessAddObjVsNav(animINST *pInst, const dsTSTRING<char> &objName,
                                    unsigned int flagsBlockedWP, unsigned short flagsDisabledEdge)
{
    if (!IsAcceptProcessing(pInst))
        return;

    aiPlanner->GetNavManager()->AutolinkInstWPs(pInst);

    m3dOBB obb;
    if (!aiPHYS_SMR::GetOBB(pInst, obb, objName))
        return;

    apCL cl;
    cl.line = 118;
    cl.file = "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_plan_phys_smr.cpp";

    dsVECTOR<navGPH::navBLOCK_OBB, 8> blockList;
    memset(&blockList, 0, 12);
    blockList.__cl = cl;

    navGPH::navBLOCK_OBB blocker;
    memcpy(&blocker, &obb, sizeof(m3dOBB));
    blocker.pInstOwner = pInst;
    blockList.PushBack(blocker);

    aiPlanner->GetNavManager()->BlockByVolList(blockList, flagsBlockedWP, flagsDisabledEdge);

    dlFree(blockList.pData);
}
