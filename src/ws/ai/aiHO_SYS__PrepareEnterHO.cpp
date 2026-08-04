#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/mdl/mdlITRC_HO.h"
#include "../../headers/ws/gm/gmHO_CTRL_BASE.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern unsigned char byte_8200155A; // shared assert-info byte @0x8200155A

// aiHO_SYS::PrepareEnterHO @ 0x832A5CF8
// ?PrepareEnterHO@aiHO_SYS@@MAA_NPAVmdlITRC_HO@@_N@Z
//
// Seed the brain's body-state bit that flags the pending HO's snap side from the controller's
// stateOut: set bit 0x1000 when the HO is NOT snapped-in (stateOut bit0 clear), else clear it.
// Always returns true. `is2D` is accepted per the ABI but unused by this build.
bool aiHO_SYS::PrepareEnterHO(mdlITRC_HO *itrcHO, bool is2D)
{
    (void)is2D;
    if (!IGNORE_STRONG_ASSERT && !itrcHO)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "itrcHO",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_ho_sys.cpp", 876, byte_8200155A);
    if (!IGNORE_STRONG_ASSERT && !itrcHO->hoCtrl)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "itrcHO->hoCtrl",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_ho_sys.cpp", 877, byte_8200155A);

    aiSTATUS *status = pBrain->st.pointee;
    unsigned int newBody = status->body.val & 0xFFFFEFFF;
    if ((itrcHO->hoCtrl->params.stateOut.state & 1) == 0)
        newBody = status->body.val | 0x1000;
    status->body.val = newBody;
    return true;
}
