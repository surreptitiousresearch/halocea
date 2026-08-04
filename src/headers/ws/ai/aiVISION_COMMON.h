#pragma once
#include "aiVISION_IFACE.h"
#include "AI_VIST.h"
#include "CONST_ARRAY_m3dV.h"
#include "aiVIS_POINTS.h"
#include "../ds/dsFLAGS.h"
#include "../ds/dsTSTRING.h"
#include "../m3d/m3dV.h"
// ws-engine ai08: aiVISION_COMMON — the shared battle/idle vision arm. DB-verified layout
// (headers_ref aiVISION_COMMON) — size 0x10C: aiVISION_IFACE base @0x00 (0x08), then the members
// below.

struct aiVIS_REQ_PARAMS; // aiVIS_REQ_PARAMS.h (used by value in PostVisRequest)

struct aiVISION_COMMON : aiVISION_IFACE {
    dsFLAGS<AI_VIST, int> state;             // 0x08 vision state bit flags
    m3dV                  dirSight;          // 0x0C current sight direction
    float                 viewPercent;       // 0x18 current visibility percentage
    float                 viewLatestTest;    // 0x1C last stored test result percentage
    CONST_ARRAY_m3dV<10>  pointsFree;        // 0x20 free-space sample points
    float                 timerKeepInvisible;// 0x9C
    m3dV                  posEye;            // 0xA0 cached viewer eye position
    aiVIS_POINTS          paramsLastReq;     // 0xAC sight points of the last posted request

    // --- virtuals / helpers reversed or reached by this batch ---

    // vtbl+0x3C — store `percent` as the latest test result. boundary (body external).
    void StoreLatestTest(float percent);
    // vtbl — true while the arm must report the target invisible (keep-invisible timer). boundary.
    bool ShouldKeepInvisible();

    // 0x83290C20 (?ResultInstantly@aiVISION_COMMON@@IAAXM@Z) — store `val` as both the latest test
    // and the current view percent. REVERSED: src/ws/ai/aiVISION_COMMON__ResultInstantly.cpp
    void ResultInstantly(float val);
    // 0x83291130 (?CalculateSightDir@aiVISION_COMMON@@IBA?AUm3dV@@XZ) — const, sret: the effective
    // sight direction (face dir, or a right/up-derived dir when the sight is clipped up/down).
    // REVERSED: src/ws/ai/aiVISION_COMMON__CalculateSightDir.cpp
    m3dV *CalculateSightDir(m3dV *out) const;
    // 0x832912F0 (?ConsiderPostingVisRequest@aiVISION_COMMON@@MAAXXZ) — protected virtual: if a
    // real vis-query is pending, run the potential-visibility / Monte-Carlo checks and either store
    // a result or post a full request. REVERSED: src/ws/ai/aiVISION_COMMON__ConsiderPostingVisRequest.cpp
    void ConsiderPostingVisRequest();
    // 0x83291448 (?PostVisRequest@aiVISION_COMMON@@MAAXXZ) — protected virtual: post a line-of-sight
    // request to the planner's def-CDT system. REVERSED: src/ws/ai/aiVISION_COMMON__PostVisRequest.cpp
    void PostVisRequest();
    // 0x832914E8 (?IsPotentiallyVisible@aiVISION_COMMON@@UBA_NABUm3dV@@0@Z) — virtual const: cheap
    // pre-check — target within straight sight radius and inside the double sight cone (or eagle-eye).
    // REVERSED: src/ws/ai/aiVISION_COMMON__IsPotentiallyVisible.cpp
    bool IsPotentiallyVisible(const m3dV &posEye, const m3dV &posCheck) const;
    // 0x832915D8 (?IsVisibleByMonteCarlo@aiVISION_COMMON@@MAA_NXZ) — protected virtual: probabilistic
    // early-visible result (only above a unit-count threshold, ~80% chance).
    // REVERSED: src/ws/ai/aiVISION_COMMON__IsVisibleByMonteCarlo.cpp
    bool IsVisibleByMonteCarlo();
    // 0x83291698 (?SetPar@aiVISION_COMMON@@UAA_NABV?$dsTSTRING@D@@M@Z) — virtual: apply a named
    // sense/tunable (angleSee*/radSee*/isEagleEye). Returns true if consumed.
    // REVERSED: src/ws/ai/aiVISION_COMMON__SetPar.cpp
    bool SetPar(const dsTSTRING<char> &key, float val);

    // 0x83291DD8 (??0aiVISION_COMMON@@QAA@PAVaiBRAIN@@@Z) — construct bound to `brain`.
    // boundary — body external to this batch.
    aiVISION_COMMON(aiBRAIN *brain);

    // vtbl+0x28 — 0x83291680 (?SetVPRaisingSpeedCurve@aiVISION_COMMON@@UAAXABV?$dsTSTRING@D@@@Z):
    // parse `curve` into the status view-percent-raising-speed spline. boundary — body external.
    void SetVPRaisingSpeedCurve(const dsTSTRING<char> &curve);

    // 0x832927D8 (?ProcessInit@aiVISION_COMMON@@UAAXXZ) — virtual: per-spawn init (clip flags, reset
    // view percent, default raising-speed curve). REVERSED: src/ws/ai/aiVISION_COMMON__ProcessInit.cpp
    void ProcessInit();
    // 0x83292718 (?ProcessFrame@aiVISION_COMMON@@UAAXXZ) — virtual: per-frame update (sight dir,
    // view percent, export). REVERSED: src/ws/ai/aiVISION_COMMON__ProcessFrame.cpp
    void ProcessFrame();
    // 0x83292438 (?UpdateViewPercent@aiVISION_COMMON@@IAAXXZ) — protected: run the vision test chain
    // unless an instant rule short-circuits it. REVERSED: src/ws/ai/aiVISION_COMMON__UpdateViewPercent.cpp
    void UpdateViewPercent();
    // 0x832924B0 (?UpdateLatestTest@aiVISION_COMMON@@MAAXXZ) — protected virtual: pull the latest CDT
    // visibility result. REVERSED: src/ws/ai/aiVISION_COMMON__UpdateLatestTest.cpp
    void UpdateLatestTest();
    // 0x83291888 (?MixesLatestToOut@aiVISION_COMMON@@MAAXXZ) — protected virtual: relax the reported
    // view-percent toward the latest test. REVERSED: src/ws/ai/aiVISION_COMMON__MixesLatestToOut.cpp
    void MixesLatestToOut();
    // 0x83292260 (?ExportAll@aiVISION_COMMON@@IAAXXZ) — protected: publish view-percent/results into
    // the status enemy block + cheap-vision cache. REVERSED: src/ws/ai/aiVISION_COMMON__ExportAll.cpp
    void ExportAll();
    // 0x83292340 (?ApplyInstantRules@aiVISION_COMMON@@IAA_NXZ) — protected: force full/zero visibility
    // for degenerate enemy states; true if handled. REVERSED: src/ws/ai/aiVISION_COMMON__ApplyInstantRules.cpp
    bool ApplyInstantRules();
    // 0x83292608 (?SetForceVision@aiVISION_COMMON@@UAAXM@Z) — virtual: pin the reported view-percent
    // to `value` and export. REVERSED: src/ws/ai/aiVISION_COMMON__SetForceVision.cpp
    void SetForceVision(float value);
    // 0x8329196C-region (?CalculateLostSightTime@aiVISION_COMMON@@MBAMM@Z) — protected virtual const:
    // base lost-sight time; overridden by aiVISION_BATTLE. boundary — body external to this batch.
    float CalculateLostSightTime(float dist) const;
};
