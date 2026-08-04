#include "../../headers/ws/ai/aiPHYS_SMR.h"
#include "../../headers/ws/ai/aiPHYS_SMR_boundaries.h" // physRB_BASE (partial) + userData/GetPhysOBB
#include "../../headers/ws/ai/aiPLANNER.h"             // aiPlanner global + GetNavManager
extern aiPLANNER *aiPlanner;   // ai08 global planner singleton
#include "../../headers/ws/nav/navMANAGER.h"           // Block/AutolinkInstWPs + navGPH::navBLOCK_OBB
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL_WRP.h"
#include "../../headers/apCL.h"
#include <string.h>

extern void dlFree(void *ptr); // ws-engine allocator free (C++ linkage, per aiBRAIN_boundary.h)

// aiPHYS_SMR::ProcessRbStartRest @ 0x83238B40
// ?ProcessRbStartRest@aiPHYS_SMR@@QAAXPAVphysRB_BASE@@@Z
//
// A physics rigid body has come to rest. Grab the object's current world OBB (expanded slightly),
// wrap it in a one-element nav-blocker list owning the object's anim instance, block the surrounding
// nav volumes (WP-block flags 2, edge-disable flags 4), then autolink the object's own waypoints so
// AI can path onto/around it.
void aiPHYS_SMR::ProcessRbStartRest(physRB_BASE *pRbBase)
{
    static apCOUNTER_TIME_CALL localTimeCounter("process_frame/ai/planner/ProcessRbStartRest");
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL> timerScope(&localTimeCounter);

    animINST *pInst = pRbBase->userData.pInst;

    m3dOBB obb;
    pRbBase->GetPhysOBB(obb);
    obb.Expand(0.050000001f);

    apCL cl;
    cl.line = 92;
    cl.file = "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_plan_phys_smr.cpp";

    // Inline-constructed nav-blocker vector: zero the {pData,nElem,allocated} head, adopt the
    // call-site cookie, then push the OBB blocker owning this instance.
    dsVECTOR<navGPH::navBLOCK_OBB, 8> blockList;
    memset(&blockList, 0, 12);
    blockList.__cl = cl;

    navGPH::navBLOCK_OBB blocker;
    memcpy(&blocker, &obb, sizeof(m3dOBB));
    blocker.pInstOwner = pInst;
    blockList.PushBack(blocker);

    aiPlanner->GetNavManager()->BlockByVolList(blockList, 2u, 4u);
    aiPlanner->GetNavManager()->AutolinkInstWPs(pInst);

    dlFree(blockList.pData);
}
