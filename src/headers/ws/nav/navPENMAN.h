#pragma once
#include "../m3d/m3dV.h"
// ws-engine nav: the penalty/pen manager — tracks dynamic navigation penalties (grenades,
// removed units). Boundary subset for this wave: only the methods the ai08 planner dispatches
// are declared (the full layout — idxMain/pts/events — is the next frontier).

struct entENTITY; // entENTITY.h
struct aiBRAIN;   // ai08 — per-unit AI brain   boundary (fwd)

struct navPENMAN {
    // 0x83224118 (?AddUnit@navPENMAN@@QAAXPAVaiBRAIN@@@Z) — begin tracking penalties for the newly
    // localised unit `pBrain`. Used by aiMIND::SetupInitial.  boundary — body external to this batch.
    void AddUnit(aiBRAIN *pBrain);

    // 0x... (?AddGrenade@navPENMAN@@QAAXPAVentENTITY@@ABUm3dV@@@Z) — add a grenade hazard at
    // `pos`.  boundary — body external to this batch.
    void AddGrenade(entENTITY *grenade, const m3dV &pos);

    // Remove any penalties/tracking associated with `unit`.  boundary.
    void RemoveUnit(entENTITY *unit);

    // Begin tracking dynamic penalties for nav-system `nsID`.  boundary.
    void StartMonitoringNavSys(short nsID);

    // 0x... (?StopMonitoringNavSys@navPENMAN@@QAAXF@Z) — stop tracking penalties for `nsID`. boundary.
    void StopMonitoringNavSys(short nsID);

    // 0x832213F0 (?CalcGrenadePenalty@navPENMAN@@SAMM@Z) — static: path penalty contribution for a
    // point at distance `dist` from a tracked grenade.  boundary.
    static float CalcGrenadePenalty(float dist);
    // 0x83224A88 (?ProcessFrame@navPENMAN@@QAAXM@Z) — per-frame penalty ageing. boundary.
    void ProcessFrame(float delta);
    // 0x832236B8 (?DropAll@navPENMAN@@QAAXXZ) — drop all tracked penalties. boundary.
    void DropAll();
    // 0x83223568 (?DebugRender@navPENMAN@@QAAXPAVaiBRAIN@@@Z) — debug overlay for `watchee`. boundary.
    void DebugRender(struct aiBRAIN *watchee);

    // 0x832244B0 (?GetPenalty@navPENMAN@@QAAMPAVnavWP@@@Z) — current dynamic penalty accrued at
    // waypoint `wp`. boundary.
    float GetPenalty(struct navWP *wp);
};
