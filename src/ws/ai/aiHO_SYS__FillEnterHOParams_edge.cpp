#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/cdt/cdtHO_PARAMS.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../../headers/ws/fsm/fsmBODY_FSM_EVENT.h"

extern unsigned char byte_8200155A; // shared assert-info byte @0x8200155A

// aiHO_SYS::FillEnterHOParams @ 0x832A69B0
// ?FillEnterHOParams@aiHO_SYS@@MAAXPAVmdlITRC_HO@@PAVhoEDGE@@AAVcdtHO_PARAMS@@@Z
//
// Fill `params` for a side-change onto `edgeNew` (from `prevItrc`): run the body's ho-enter-param
// FSM callback so the body can populate the block. `prevItrc` / `edgeNew` are validated only.
void aiHO_SYS::FillEnterHOParams(mdlITRC_HO *prevItrc, hoEDGE *edgeNew, cdtHO_PARAMS &params)
{
    if (!IGNORE_STRONG_ASSERT && !prevItrc)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "prevItrc",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_ho_sys.cpp", 931, byte_8200155A);
    if (!IGNORE_STRONG_ASSERT && !edgeNew)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "edgeNew",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_ho_sys.cpp", 932, byte_8200155A);

    cdtHO_PARAMS work;
    work = params;
    aiBODY_IFACE *body = pBrain->body.pointee;
    body->ActivateBodyCB(&fsmBODY_FSM_EVENT::get_ho_enter_param_(), &work);
    params = work;
}
