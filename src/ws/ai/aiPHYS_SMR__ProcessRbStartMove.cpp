#include "../../headers/ws/ai/aiPHYS_SMR.h"
#include "../../headers/ws/ai/aiPHYS_SMR_boundaries.h"      // physRB_BASE::previousObb / userData.pInst
#include "../../headers/ws/ai/aiPLANNER.h"                  // aiPLANNER::GetNavManager
#include "../../headers/ws/nav/navMANAGER.h"                // Unblock/DisconnectInstWPs
#include "../../headers/ws/nav/navBLOCK_OBB.h"              // navGPH::navBLOCK_OBB
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/m3d/m3dOBB.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL_WRP.h"    // RAII profiling scope
#include <string.h>

// The ai08 planner singleton (aiEXIFACE.h).
extern aiPLANNER *aiPlanner;

// aiPHYS_SMR::ProcessRbStartMove @ 0x83238A00
// ?ProcessRbStartMove@aiPHYS_SMR@@QAAXPAVphysRB_BASE@@@Z
//
// A rigid body just started moving: the nav volumes its previous-frame OBB occupied must be
// unblocked and the object's waypoints disconnected so paths can route through the now-vacated
// space again. Mirror of ProcessRbStartRest.
void aiPHYS_SMR::ProcessRbStartMove(physRB_BASE *pRbBase)
{
    static apCOUNTER_TIME_CALL localTimeCounter("process_frame/ai/planner/ProcessRbStartMove");
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL> profileScope(&localTimeCounter);

    animINST *pInst = pRbBase->userData.pInst;

    m3dOBB obb = pRbBase->previousObb;
    obb.Expand(0.050000001f);

    dsVECTOR<navGPH::navBLOCK_OBB, 8> volList;
    navGPH::navBLOCK_OBB block;
    memcpy(&block.vol, &obb, sizeof(m3dOBB));
    block.pInstOwner = nullptr;
    volList.PushBack(block);

    aiPlanner->GetNavManager()->UnblockByVolList(volList, 2, 4);
    aiPlanner->GetNavManager()->DisconnectInstWPs(pInst);
}
