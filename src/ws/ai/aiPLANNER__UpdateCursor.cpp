// aiPLANNER::UpdateCursor @ 0x83246828
// ?UpdateCursor@aiPLANNER@@IAAXXZ
//
// Protected: when either the nav editor is active or the console's "fast-peek select" mode is on,
// cast a debug pick-ray forward from the debug camera and, on a hit, notify the nav editor of the
// crossed geometry. In fast-peek mode, if no brain is currently watched and the picked entity is an
// aiBRAIN, select it as the debug watchee and mirror that to the console.
//
// The ray uses a cdtREFINE_CURSOR filter: the compiler constructs a base cdtREFINE (flags=0,
// layerMask=0xFFFFFFFF, stateObjUsrIncl=0x20, self=nullptr) then overwrites its vptr with
// cdtREFINE_CURSOR's vtable — replicated here to stay faithful to the emitted code.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB.h"
#include "../../headers/ws/ai/aiPLANNER_debugkeys_boundaries.h" // _aiConCb
#include "../../headers/ws/ned/nedNAV_EDITOR_boundary.h"
#include "../../headers/ws/msg/msgADDR.h"                       // msgADDR (gsIs param)
#include "../../headers/ws/cam/camCAMERA.h"
#include "../../headers/ws/cdt/cdtREFINE.h"
#include "../../headers/ws/cdt/cdtINFO.h"
#include "../../headers/ws/gs/gsGEOM_ANCHOR.h"
#include "../../headers/ws/gs/gsSR_MANAGER.h"                   // srMANAGER::ShootRay, pSrMng
#include "../../headers/ws/m3d/m3dMATR.h"
#include "../../headers/apCL.h"

struct aiBRAIN;
extern aiBRAIN *_pBrainDebugWatchee;

// ?$gsIs@VaiBRAIN@@...@@YAHPBVmsgADDR@@@Z — RTTI type test.  boundary.
template <class T> int gsIs(const msgADDR *addr);

// &cdtREFINE_CURSOR::`vftable' — the pick-cursor refine filter's vtable.  boundary.
extern cdtREFINE_vtbl cdtREFINE_CURSOR__vftable;

void aiPLANNER::UpdateCursor()
{
    bool editorEnabled = navEditor->IsEnabled();
    bool fastPeek = (_aiConCb != nullptr) && _aiConCb->wSelectFastPeek;

    if (!(editorEnabled || fastPeek))
        return;

    camCAMERA *cam = gsCameraDebug();
    m3dMATR camMatr;
    cam->GetMatrC2WLeft(&camMatr);

    m3dV vFrom = { camMatr.__s1._41, camMatr.__s1._42, camMatr.__s1._43 }; // camera origin (row 4)
    m3dV vDir  = { camMatr.__s1._31, camMatr.__s1._32, camMatr.__s1._33 }; // camera forward z (row 3)

    // DEVIATION: the packed literal 0xFFFFFFFF00000020 never existed — it was the folded 3-arg
    // ctor model fusing two independent registers. The caller sets `li r5, -1` @0x832468C4
    // (layerMask), and `li r10, 0x20` @0x832468B0 / `std r10, var_130` / `ld r6, 0(r11)`
    // @0x832468CC (stateObjUsrIncl = 0x20, one 64-bit GPR), with `li r7, 0` @0x832468BC.
    cdtREFINE refine(0, 0xFFFFFFFFu, apSTATE_T<int64_t>{0x20}, nullptr);
    refine.__vftable = &cdtREFINE_CURSOR__vftable; // promote to cdtREFINE_CURSOR

    cdtINFO info;
    apCL cl = { "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_plan_msg.cpp", 0x1A0 };

    if (pSrMng->ShootRay(vFrom, vDir, 500.0f, &refine, &info, cl))
    {
        gsGEOM_ANCHOR anchor(info);
        navEditor->NoticePlrCross(anchor, vDir);

        if (fastPeek && !_pBrainDebugWatchee &&
            gsIs<aiBRAIN>(reinterpret_cast<const msgADDR *>(anchor.GetEnt())))
        {
            _pBrainDebugWatchee = reinterpret_cast<aiBRAIN *>(anchor.GetEnt());
            if (_aiConCb)
                _aiConCb->SendWatcheeToConsole(false);
        }
    }
}
