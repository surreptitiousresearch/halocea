#pragma once
#include "../m3d/m3dOBB.h"
#include "../ds/dsTSTRING.h"
// ws-engine ai08 (a8_plan_phys_smr) — the "SMR" (spawn-movement-restrict) physics helper owned by
// aiPLANNER at +0x11A0. When a physics rigid body (a movable/destructible object) starts to rest or
// move, it re-blocks / re-links the nav graph around the object's oriented bounding box so AI paths
// route around it. `isEnabledBySSL` gates the whole feature from script.
// DB-verified layout (types_members aiPHYS_SMR): __vftable@0, timerIgnore@4, isEnabledBySSL@8 — size 12.

struct aiPHYS_SMR_vtbl; // ai08 — vtable                     boundary (fwd)
struct animINST;        // anim instance                     boundary (fwd)
struct physRB_BASE;     // phys rigid body                   boundary (fwd)

struct aiPHYS_SMR {
    aiPHYS_SMR_vtbl *__vftable;    // 0x00
    float            timerIgnore;  // 0x04 grace timer after a spawn during which SMR is skipped
    bool             isEnabledBySSL; // 0x08 script master enable

    // ?ProcessRbStartRest@aiPHYS_SMR@@QAAXPAVphysRB_BASE@@@Z @ 0x83238B40 — a body came to rest:
    // block the nav volumes it now occupies and autolink the object's waypoints.
    void ProcessRbStartRest(physRB_BASE *pRbBase);
    // ?ProcessRbStartMove@aiPHYS_SMR@@QAAXPAVphysRB_BASE@@@Z @ 0x83238A00 — a body started moving:
    // unblock the nav volumes its previous OBB occupied and disconnect the object's waypoints.
    void ProcessRbStartMove(physRB_BASE *pRbBase);
    // ?ProcessAddObjVsNav@aiPHYS_SMR@@QAAXPAVanimINST@@ABV?$dsTSTRING@D@@KG@Z @ 0x83238C80 —
    // autolink `pInst`'s waypoints and block the named sub-object's OBB against the nav graph.
    void ProcessAddObjVsNav(animINST *pInst, const dsTSTRING<char> &objName,
                            unsigned int flagsBlockedWP, unsigned short flagsDisabledEdge);
    // ?ProcessRemoveObjVsNav@aiPHYS_SMR@@QAAXPAVanimINST@@ABV?$dsTSTRING@D@@KG@Z @ 0x83238D68 —
    // inverse of ProcessAddObjVsNav: disconnect `pInst`'s waypoints and unblock the OBB.
    void ProcessRemoveObjVsNav(animINST *pInst, const dsTSTRING<char> &objName,
                               unsigned int flagsBlockedWP, unsigned short flagsDisabledEdge);

    // ?IsAcceptProcessing@aiPHYS_SMR@@IAA_NPAVanimINST@@@Z @ 0x83238980 — should `pInst` be
    // processed at all (feature enabled, grace timer elapsed, instance has the SMR feature)? boundary.
    bool IsAcceptProcessing(animINST *pInst);
    // ?GetOBB@aiPHYS_SMR@@SA_NPAVanimINST@@AAVm3dOBB@@ABV?$dsTSTRING@D@@@Z @ 0x832388A0 — static:
    // fetch the named sub-object's oriented bounding box; false when absent. boundary.
    static bool GetOBB(animINST *pInst, m3dOBB &out, const dsTSTRING<char> &objName);
    // 0x83238840 (?ProcessINIT_LEVEL@aiPHYS_SMR@@QAAXXZ) — per-level init. boundary.
    void ProcessINIT_LEVEL();
    // 0x83238858 (?UpdateTimers@aiPHYS_SMR@@IAAXM@Z) — protected: age the SMR grace timers. boundary.
    void UpdateTimers(float delta);
    // 0x83238870 (?HaveSMRFeature@aiPHYS_SMR@@SA_NPAVanimINST@@@Z) — static: does `pInst` carry the
    // SMR feature? boundary.
    static bool HaveSMRFeature(animINST *pInst);
};
