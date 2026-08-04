#pragma once
#include "aiGOAL_IFACE.h"
#include "../ds/dsVECTOR.h"
#include "../m3d/m3dV.h"
// ws-engine ai08 (a8_goal.cpp): concrete goal/navigation-movement module for a brain.
// DB-verified layout (types_members aiGOAL): base aiGOAL_IFACE@0 (60), navSys@0x3C, pointRoam@0x40,
// timerChangeRoam@0x4C, timerStuckEvent@0x50, speedList@0x54 (dsVECTOR<aiSPD,8>, 20),
// forcedSpeed@0x68 (aiSPD, 52) — size 0x9C.

struct aiBRAIN; // ai08 — owning brain (fwd, ctor arg)
struct navSYS;  // nav subsystem (fwd, pointer member / arg)
struct navPATH; // nav path       (fwd, arg)
struct m3dV;    // 3-vector

struct aiGOAL : aiGOAL_IFACE {
    // ??0aiGOAL@@QAA@PAVaiBRAIN@@@Z — construct the goal module owned by `brain` (SetupMind).
    aiGOAL(aiBRAIN *brain);

    navSYS               *navSys;          // 0x3C current nav system (owned via ref-count)
    m3dV                  pointRoam;       // 0x40 current roam target
    float                 timerChangeRoam; // 0x4C
    float                 timerStuckEvent; // 0x50
    dsVECTOR<aiSPD, 8>    speedList;       // 0x54 available gait descriptors
    aiSPD                 forcedSpeed;     // 0x68 currently forced gait

    // 0x832AF040 (?GetRandomPL_ID@aiGOAL@@UAAFXZ) — a random valid path-link (waypoint) id from the
    // current nav system, or -1 when there is none. REVERSED: aiGOAL__GetRandomPL_ID.cpp.
    short GetRandomPL_ID();
    // 0x832AF0C0 (?BuildFreePath@aiGOAL@@UAA_NFPAVnavPATH@@@Z) — build a penalty/HO/used-path-aware
    // path to waypoint `plid` into `pPath`. REVERSED: aiGOAL__BuildFreePath.cpp.
    bool BuildFreePath(short plid, navPATH *pPath);
    // 0x832AF1A8 (?UpdateTimers@aiGOAL@@UAAXM@Z) — advance this module's roam/stuck timers by `dt`.
    // REVERSED: aiGOAL__UpdateTimers.cpp.
    void UpdateTimers(float dt);
    // 0x832AF488 (?UpdatePosReached@aiGOAL@@IAAX_N@Z) — protected: set/clear the status "position
    // reached" flag and accumulate the reached-time. REVERSED: aiGOAL__UpdatePosReached.cpp.
    void UpdatePosReached(bool use2DNavigation);
    // 0x832AF520 (?IsPointClose@aiGOAL@@UAA_NABUm3dV@@_N@Z) — true when the body is within the command
    // reach distance of `goal` (2-D XZ + height gate when use2DNavigation). REVERSED: aiGOAL__IsPointClose.cpp.
    bool IsPointClose(const m3dV &goal, bool use2DNavigation);
    // 0x832AF6A8 (?AnalyzeMove@aiGOAL@@UAAX_N@Z) — per-frame movement analysis: pos-reached, stuck
    // flags/event, and (2-D) touch reaction. REVERSED: aiGOAL__AnalyzeMove.cpp.
    void AnalyzeMove(bool is2D);
    // 0x832AF790 (?SetNavSystem@aiGOAL@@UAAXPAVnavSYS@@_N@Z) — switch the current nav system (ref-count
    // release/add, register with the chaser, notify the coord group). REVERSED: aiGOAL__SetNavSystem.cpp.
    void SetNavSystem(navSYS *ns, bool doNotify);

    // --- callees (bodies external to this batch). boundary ---
    // 0x832AEFE0 (?ForcedClear@aiGOAL@@UAAXXZ) — virtual: release the owned nav-system (dropping the
    // brain's reference) and clear the pointer. REVERSED: aiGOAL__ForcedClear.cpp.
    void ForcedClear();

    void UpdateStuckFlags(); // refresh the status stuck bitflags from body motion
    void UpdateStuckEvent(); // fire/clear the stuck-event timer
    void ReactToTouch();     // 2-D touch-reaction bookkeeping
};
