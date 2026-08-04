#pragma once
// ws-engine ai08 — "chaser" planner helper: tracks where a target (usually the player)
// was last expected/seen and drives investigation paths through a per-nav-system
// suspicion cache (nsSUSCACHE). DB-verified layout (types_members aiCHASER) — size 40 (0x28).

#include "aiSUSTYPE.h"
#include "../m3d/m3dV.h"

struct aiBRAIN;      // aiBRAIN.h
struct navSYS;       // nav/navSYS.h
struct navPATH;      // nav/navPATH.h
struct nsSUSCACHE;   // nsSUSCACHE.h
template<class T, int N> struct dsVECTOR;

struct aiCHASER {
    m3dV  posExpectedPlayer;   // 0x00
    m3dV  posRecentDeath;      // 0x0C
    float timerRecentDeath;    // 0x18
    float timerSawPlayer;      // 0x1C
    float timerSparseUpPlrPos; // 0x20
    int   idxLastUpdatedCache; // 0x24

    // Locate the suspicion cache bound to `sys` (nullptr when none). ai08 boundary.
    nsSUSCACHE *FindCache(navSYS *sys);

    // 0x8320B590 — penalty [0..3] scaled by distance from the recent-death position, or 0
    // when the recent-death timer has expired.
    float CalcRecentDeathPenalty(const m3dV *pos);

    // Refresh the cached view positions for every tracked suspicion point (per-frame). Body
    // external to this batch. boundary — called by aiCACHER::ProcessFRAME.
    void UpdateViewPositions();

    // 0x8320B668 — count all chaser timers down by `time` (unless AI is off).
    void UpdateTimers(float time);

    // 0x8320BEC0 — stop monitoring + drop the last-updated nav-system's suspicion cache.
    void ForgetUnusedNavSys();

    // 0x8320BF20 — clear every cached suspicion point and reset the cache index.
    void DropAll();

    // 0x8320C1E8 — record a corpse/death position reported by `pBrain` into the cache.
    void NoticeDeath(aiBRAIN *pBrain);

    // 0x8320C2A8 — record a heard-player position for nav-system `sys`.
    void ReportHeardPlayer(const m3dV *pos, navSYS *sys);

    // 0x8320C328 — true when there is any cached chase data better than a random point
    // for `pBrain`'s team/nav-system.
    int HasSomeChaseData(aiBRAIN *pBrain);

    // 0x8320C3C0 — build a nav path from `pBrain` to the current best cached view point.
    bool RequestPathToBestView(aiBRAIN *pBrain, navPATH &path);

    // 0x8320C560 — collect the best cached view waypoints for `pBrain` into `wps`.
    bool GetBestViewPoints(aiBRAIN *pBrain, dsVECTOR<short, 8> &wps);

    // 0x8320C630 — debug-draw the tracked suspicion points for `watchee`.
    void DebugRender(aiBRAIN *watchee);
    // Debug-draw the points of one suspicion type. ai08 boundary (overload).
    void DebugRender(aiSUSTYPE type, aiBRAIN *watchee);

    // 0x8320CC10 — register nav-system `sys`, creating its suspicion cache if absent.
    void RegisterNavSys(navSYS *sys);
};
