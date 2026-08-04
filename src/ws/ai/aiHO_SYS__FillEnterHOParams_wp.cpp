#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/cdt/cdtHO_PARAMS.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/fsm/fsmBODY_FSM_EVENT.h"

// aiHO_SYS::FillEnterHOParams @ 0x832A6860
// ?FillEnterHOParams@aiHO_SYS@@MAAXAAVnavWP@@_NAAVcdtHO_PARAMS@@@Z
//
// Fill `params` for entering the HO at waypoint `wp`: run the body's ho-enter-param FSM callback to
// let the body populate the block, then stamp the waypoint's enter frame (origin/forward/up) and
// its debug identity into the enter-params sub-block. `is2D` is accepted per the ABI but unused here.
void aiHO_SYS::FillEnterHOParams(navWP &wp, bool is2D, cdtHO_PARAMS &params)
{
    (void)is2D;

    cdtHO_PARAMS work;
    work = params;
    aiBODY_IFACE *body = pBrain->body.pointee;
    body->ActivateBodyCB(&fsmBODY_FSM_EVENT::get_ho_enter_param_(), &work);
    params = work;

    m3dV dirUp;
    wp.GetDirUp(&dirUp);
    m3dV dirToPlane = *wp.GetDirToHOPlane();
    m3dV effectivePos;
    wp.GetEffectivePos(&effectivePos);

    {
        dsTSTRING<char> name(wp.GetName());
        params.enterParams.SetWpInfo(wp.GetParentObject(), &name);
    }
    params.enterParams.Init(effectivePos, dirToPlane, dirUp, 2u);
}
