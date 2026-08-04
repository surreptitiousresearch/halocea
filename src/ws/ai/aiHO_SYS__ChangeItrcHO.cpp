#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/cdt/cdtHO_PARAMS.h"
#include "../../headers/ws/gm/gmHIDEOUT_SYS.h"
#include "../../headers/ws/mdl/mdlITRC_HO.h"
#include "../../headers/ws/ho/hoEDGE.h"
#include "../../headers/ws/ho/HO_CHANGE_TYPE.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern unsigned char byte_8200155A; // shared assert-info byte @0x8200155A
extern int IGNORE_STRONG_ASSERT;    // ?IGNORE_STRONG_ASSERT@@3HA

// aiHO_SYS::ChangeItrcHO @ 0x832A5F18
// ?ChangeItrcHO@aiHO_SYS@@UAAPAVmdlITRC_HO@@PAV2@PAVhoEDGE@@AAW4HO_CHANGE_TYPE@@@Z
//
// Request a hideout side-change from `prevItrc` onto `edgeNew`: fill the enter params, ask the
// hideout system for the new controller, and wrap it in a fresh HO itrc. Returns nullptr if the
// change is rejected.
mdlITRC_HO *aiHO_SYS::ChangeItrcHO(mdlITRC_HO *prevItrc, hoEDGE *edgeNew, HO_CHANGE_TYPE &changeType)
{
    if (!IGNORE_STRONG_ASSERT && !prevItrc)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "prevItrc",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_ho_sys.cpp", 561, byte_8200155A);
    if (!IGNORE_STRONG_ASSERT && !edgeNew)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "edgeNew",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_ho_sys.cpp", 562, byte_8200155A);

    cdtHO_PARAMS params;
    FillEnterHOParams(prevItrc, edgeNew, params);
    gmHO_CTRL_BASE *ctrl =
        gmHideOutSys->ChangeHideout_2(&params, prevItrc->hoCtrl, edgeNew, &changeType);
    if (!ctrl)
        return nullptr;
    return gmHideOutSys->NewItrcHo_2(ctrl);
}
