#pragma once
#include "aiHO_SYS_BASE.h"      // base class + HO_SHIFT enum
#include "../m3d/m3dV.h"        // GetHOWantPos returns m3dV by value
#include "../ds/dsSHARED_PTR.h" // the HO ITRC handles are dsSHARED_PTR<mdlITRC_BASE,...> (by value)

struct mdlITRC_BASE;   // mdl — interactive transform ref component (mdlITRC_BASE.h)
struct mdlITRC_HO;     // mdl — HO instance trace context               boundary (opaque ptr)
enum HO_CHANGE_TYPE;   // ho — reason a hideout side-change was requested (boundary enum)
// ws-engine ai08 (a8_ho_sys.cpp): the concrete "hint object" (HO) query subsystem a brain uses to
// pick cover / vantage waypoints. DB-verified layout (headers_ref aiHO_SYS / types_members), size
// 0x20: derives aiHO_SYS_BASE (vtbl@0), then pBrain@4, framesToLeaveHO@8, alwaysInHO@C, and four
// enter-penalty/shift floats@10..1C.

struct aiBRAIN;      // ai08 — owning brain (full layout aiBRAIN.h)  boundary (ptr)
struct navWP;        // nav waypoint (navWP.h)                        boundary (ref/ptr)
struct mdlITRC_HO;   // mdl — HO instance trace context               boundary (opaque ptr)
struct hoEDGE;       // ho subsystem — HO edge (full layout hoEDGE.h)
struct cdtHO_PARAMS; // cdt — HO enter parameter block (cdtHO_PARAMS.h)

struct aiHO_SYS : aiHO_SYS_BASE {
    aiBRAIN *pBrain;                            // 0x04
    int      framesToLeaveHO;                   // 0x08
    bool     alwaysInHO;                        // 0x0C (+3 padding)
    float    enterHOPenaltyCenterShiftToHO;     // 0x10
    float    enterHOPenaltyCenterShiftToCorner; // 0x14
    float    enterHOPosShiftToHO;               // 0x18
    float    enterHOPosShiftToCorner;           // 0x1C

    // 0x83179xxx — construct for the owning brain. Boundary ctor (ai_setup_subsystems). body external.
    aiHO_SYS(aiBRAIN *);

    // 0x832A5910 (?GetHOWantPos@aiHO_SYS@@UBA?AUm3dV@@ABVnavWP@@W4HO_SHIFT@aiHO_SYS_BASE@@@Z) —
    // virtual, const. Compute the desired world position for waypoint `howp` under `shift`. REVERSED.
    m3dV GetHOWantPos(const navWP &howp, HO_SHIFT shift) const;

    // 0x832A5B38 (?ClampPosOnEdge@aiHO_SYS@@UAAMPAVmdlITRC_HO@@PBVhoEDGE@@M@Z) — virtual. Clamp `pos`
    // onto the usable interior of `edge` (using the per-edge near/far offsets). REVERSED.
    float ClampPosOnEdge(mdlITRC_HO *itrc, const hoEDGE *edge, float pos);

    // 0x832A5C78 (?AdvCheckAcceptHO@aiHO_SYS@@UBA_NPBVnavWP@@P82@BA_NABUm3dV@@@ZAA_N@Z) — virtual,
    // const. Ask the brain's goal module whether the HO at `howp` is acceptable, using the planner's
    // reconsider checker. `checker` (a member-fn pointer) is accepted but the planner's own checker is
    // substituted. REVERSED.
    bool AdvCheckAcceptHO(const navWP *howp,
                          bool (aiHO_SYS::*checker)(const m3dV &) const,
                          bool &checkAllEnemies) const;

    // vtbl — compute the near/far usable offsets of `edge` for `itrc` (writes two floats). Body
    // external to this batch.  boundary.
    void GetEdgeOffsets(mdlITRC_HO *itrc, const hoEDGE *edge, float *outNear, float *outFar) const;

    // --- HO enter/leave bookkeeping (a8_ho_sys.cpp) — REVERSED this drain -----------------------
    typedef dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > ItrcPtr;

    // 0x832A7638 (?NoticeEnteredHO@aiHO_SYS@@UAAXV?$dsSHARED_PTR@...@@@Z) — virtual: record that the
    // body just entered the HO referenced by `itrc`; if it is not already the current one, notify the
    // owning brain (OnEnterHO). Consumes `itrc`. REVERSED.
    void NoticeEnteredHO(ItrcPtr itrc);

    // 0x832A76A8 (?IsInHO@aiHO_SYS@@UAA_NXZ) — virtual: true when the body currently holds an HO ITRC.
    bool IsInHO();

    // 0x832A79E0 (?IsInHO@aiHO_SYS@@UAA_NAAVnavWP@@@Z) — virtual: true when the current HO ITRC is the
    // one produced for waypoint `wp`. REVERSED.
    bool IsInHO(navWP &wp);

    // 0x832A78A0 (?LeaveHO@aiHO_SYS@@UAAXXZ) — virtual: request leaving the HO (unless always-in-HO):
    // schedule a leave next frame if still current, else force a leave event if peeking a hideout.
    void LeaveHO();

    // 0x832A7950 (?KeepHO@aiHO_SYS@@UAAXXZ) — virtual: cancel a pending leave while still in an HO.
    void KeepHO();

    // 0x832A7EF0 (?DoChangeHO@aiHO_SYS@@UAA?AV?$dsSHARED_PTR@...@@PAVmdlITRC_HO@@ABW4HO_CHANGE_TYPE@@@Z)
    // — virtual: adopt `itrc` as the new HO and fire the body's "change_side" hideout event; returns
    // the adopted ITRC handle. REVERSED.
    ItrcPtr DoChangeHO(mdlITRC_HO *itrc, const HO_CHANGE_TYPE &changeType);

    // 0x832A77E8 (?GetCurrentItrc@aiHO_SYS@@IAA?AV?$dsSHARED_PTR@...@@XZ) — protected: return the body's
    // current ITRC if it is an HO, else an empty handle. REVERSED.
    ItrcPtr GetCurrentItrc();

    // 0x832A7710 (?IsSameItrc@aiHO_SYS@@MBA_NV?$dsSHARED_PTR@...@@0@Z) — protected const virtual: true
    // when both handles are HO ITRCs referring to the same underlying corner. Consumes both. REVERSED.
    bool IsSameItrc(ItrcPtr itrc1, ItrcPtr itrc2) const;

    // 0x832A7E78 (?IsCurrentItrc@aiHO_SYS@@MAA_NV?$dsSHARED_PTR@...@@@Z) — protected virtual: true when
    // `itrc` is the current HO ITRC. Consumes `itrc`. REVERSED.
    bool IsCurrentItrc(ItrcPtr itrc);

    // 0x832A7B08 (?ProcessLeaveHO@aiHO_SYS@@MAAXXZ) — protected virtual: if still in an HO, send the
    // leave event and notify the brain (OnLeaveHO). REVERSED.
    void ProcessLeaveHO();

    // 0x832A7B78 (?SendEnterHOEvent@aiHO_SYS@@MAAXV?$dsSHARED_PTR@...@@@Z) — protected virtual: drive
    // the body FSM's hideout-enter event set. Body external (frontier: mdlITRC_HO layout). boundary.
    void SendEnterHOEvent(ItrcPtr itrc);

    // --- callees whose bodies are the next frontier (declared so this class's TUs close) ----------
    // 0x832A79B0 (?GetCurrentItrcHO@aiHO_SYS@@UAA?AV?$dsSHARED_PTR@...@@XZ) — virtual. boundary.
    ItrcPtr GetCurrentItrcHO();
    // 0x832A62B8 (?GetItrcHO@aiHO_SYS@@MAAPAVmdlITRC_HO@@AAVnavWP@@W4HO_SHIFT@aiHO_SYS_BASE@@M_N2@Z) —
    // protected virtual: produce/borrow the ITRC for `wp` under `shift`. boundary.
    mdlITRC_HO *GetItrcHO(navWP &wp, HO_SHIFT shift, float radius, bool b1, bool b2);
    // 0x832A5B28 (?ForceEventLeaveHO@aiHO_SYS@@UAAXXZ) — virtual. boundary.
    void ForceEventLeaveHO();
    // 0x832A67F8 (?SendLeaveHOEvent@aiHO_SYS@@MAAXXZ) — protected virtual. REVERSED.
    void SendLeaveHOEvent();

    // --- HO enter/change/frame bookkeeping (a8_ho_sys.cpp) — REVERSED this drain ------------------
    // 0x832A5CF8 (?PrepareEnterHO@aiHO_SYS@@MAA_NPAVmdlITRC_HO@@_N@Z) — protected virtual: seed the
    // brain body-state bit for the pending HO from the controller's stateOut. REVERSED.
    bool PrepareEnterHO(mdlITRC_HO *itrcHO, bool is2D);

    // 0x832A5F18 (?ChangeItrcHO@aiHO_SYS@@UAAPAVmdlITRC_HO@@PAV2@PAVhoEDGE@@AAW4HO_CHANGE_TYPE@@@Z) —
    // virtual: request a side-change from `prevItrc` onto `edgeNew`; returns the new HO itrc. REVERSED.
    mdlITRC_HO *ChangeItrcHO(mdlITRC_HO *prevItrc, hoEDGE *edgeNew, HO_CHANGE_TYPE &changeType);

    // 0x832A6098 (?IsLeftEdge@aiHO_SYS@@UAA_NPAVmdlITRC_HO@@PBVhoEDGE@@1AA_N@Z) — virtual: true when
    // `edgeLeft` is a left-neighbour of `edge`; sets `isOuterEdge`. REVERSED.
    bool IsLeftEdge(mdlITRC_HO *itrc, const hoEDGE *edge, const hoEDGE *edgeLeft, bool &isOuterEdge);

    // 0x832A61A8 (?IsRightEdge@aiHO_SYS@@UAA_NPAVmdlITRC_HO@@PBVhoEDGE@@1AA_N@Z) — virtual. REVERSED.
    bool IsRightEdge(mdlITRC_HO *itrc, const hoEDGE *edge, const hoEDGE *edgeRight, bool &isOuterEdge);

    // 0x832A6860 (?FillEnterHOParams@aiHO_SYS@@MAAXAAVnavWP@@_NAAVcdtHO_PARAMS@@@Z) — protected: fill
    // `params` for entering the HO at waypoint `wp`. REVERSED.
    void FillEnterHOParams(navWP &wp, bool is2D, cdtHO_PARAMS &params);

    // 0x832A69B0 (?FillEnterHOParams@aiHO_SYS@@MAAXPAVmdlITRC_HO@@PAVhoEDGE@@AAVcdtHO_PARAMS@@@Z) —
    // protected: fill `params` for changing from `prevItrc` onto `edgeNew`. REVERSED.
    void FillEnterHOParams(mdlITRC_HO *prevItrc, hoEDGE *edgeNew, cdtHO_PARAMS &params);

    // 0x832A6C50 (?ProcessFrame@aiHO_SYS@@UAAXM@Z) — virtual: per-frame leave-HO countdown. REVERSED.
    void ProcessFrame(float dt);

    // 0x832A6D08 (?GetHOPos@aiHO_SYS@@MAA?AUm3dV@@AAVnavWP@@_N@Z) — protected: world HO position for
    // `wp`, ground-projected via a downward ray. Returns m3dV (sret). REVERSED.
    m3dV GetHOPos(navWP &wp, bool is2D);

    // 0x832A7590 (?EnterHO@aiHO_SYS@@UAAX...dsSHARED_PTR<mdlITRC_BASE>...@Z) — virtual: adopt `itrc`
    // as the current HO (leave the old, notify the brain, fire the enter event). REVERSED.
    void EnterHO(ItrcPtr itrc);

    // 0x832A6304 (?FillEnterHOParams_2...) — protected variant used by GetItrcHO. Body external
    // (next frontier). boundary.
    void FillEnterHOParams_2(navWP *wp, bool corner, cdtHO_PARAMS *params);
};
