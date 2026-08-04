#pragma once
#include "../m3d/m3dSPL_LINEAR1D.h"
#include "../ds/LIST.h"
#include "aiS_PERF_LOAD.h"
// ws-engine ai08 — the AI level-of-detail subsystem (owned by aiPLANNER at +0x1168). Periodically
// recomputes a per-unit LOD value (nearest-player distance mapped through a response spline),
// throttles which unit may run a "mind" (think) frame this tick (the mind sparser), and tracks a
// rolling "strike performance load" budget that gates whether new strikes may be chosen.
// DB-verified layout (types_members aiLOD_SYS): timerUpdateLODs@0, curveLODValue@4 (m3dSPL_LINEAR1D),
// queueLoads@0x18 (ds::LIST<aiS_PERF_LOAD>), cachedLoadSum@0x2C, msAllowed@0x30 — size 56.

struct aiBRAIN;         // ai08 — per-unit AI brain            boundary (fwd)
struct aiWATCHER;       // ai08 — player/entity watcher        boundary (fwd)
struct aiWATCHER_BRAIN; // ai08 — AI-unit watcher              boundary (fwd)

struct aiLOD_SYS {
    float             timerUpdateLODs; // 0x00 countdown to the next full LOD recompute
    m3dSPL_LINEAR1D   curveLODValue;   // 0x04 distance -> LOD response spline
    ds::LIST<aiS_PERF_LOAD> queueLoads; // 0x18 active strike performance-load samples
    float             cachedLoadSum;   // 0x2C cached sum of queueLoads[].load

    // DB anon nested type aiLOD_SYS::<unnamed_type_msAllowed>: idx@0, pb@4 — size 8.
    struct MS_ALLOWED {
        int             idx; // 0x00 round-robin cursor into the active-unit list
        const aiBRAIN  *pb;  // 0x04 the one brain permitted a mind frame this tick
    } msAllowed;             // 0x30

    // ?CalcLODValue@aiLOD_SYS@@IBAMPAVaiWATCHER_BRAIN@@PAVaiWATCHER@@@Z — the LOD value for
    // `pBrainWatcher` given the nearest player watcher `player` (distance through curveLODValue,
    // +1 when the unit faces away from the player).
    float CalcLODValue(aiWATCHER_BRAIN *pBrainWatcher, aiWATCHER *player) const;
    // ?MayChooseStrike@aiLOD_SYS@@QAA_NPBVaiBRAIN@@@Z — may `pBrain` pick a new strike now?
    bool MayChooseStrike(const aiBRAIN *pBrain);
    // ?RecalculateLODs@aiLOD_SYS@@IAAXXZ — refresh every live unit's LOD value.
    void RecalculateLODs();
    // ?UpdateMindSparser@aiLOD_SYS@@IAAXXZ — advance the round-robin mind-frame permit.
    void UpdateMindSparser();
    // ?ShouldSkipMindFrame@aiLOD_SYS@@QAA_NPBVaiBRAIN@@@Z — skip `pBrain`'s mind frame this tick?
    int ShouldSkipMindFrame(const aiBRAIN *pBrain);
    // ?DebugRender@aiLOD_SYS@@QBAXXZ — debug overlay for the LOD state.
    void DebugRender() const;
    // ?ProcessINIT_LEVEL@aiLOD_SYS@@QAAXXZ — per-level init: parse the default LOD spline.
    void ProcessINIT_LEVEL();
    // ?DropAll@aiLOD_SYS@@QAAXXZ — clear the load queue and mind-sparser state.
    void DropAll();
    // ?UpdateStrikePerfLoad@aiLOD_SYS@@IAAXM@Z — age the load queue by `td` and re-sum.
    void UpdateStrikePerfLoad(float td);
    // ?ProcessFrame@aiLOD_SYS@@QAAXM@Z — per-frame tick.
    void ProcessFrame(float delta);
    // ?AddStrikePerfLoad@aiLOD_SYS@@QAAXABVaiS_PERF_LOAD@@@Z — enqueue a strike performance-load
    // sample. boundary — body external to this batch.
    void AddStrikePerfLoad(const aiS_PERF_LOAD &load);
};
