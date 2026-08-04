#pragma once
#include "../ia/iaIACTOR.h"

// ws-engine ai08 — the AI "cacher" actor: a per-frame planner helper registered with the message
// system that drives the watcher subsystem's player acquisition and the chaser's view/timer
// updates. DB-verified: aiCACHER derives iaIACTOR (base at offset 0), adds no new fields the
// reversed method touches.

struct aiCACHER : iaIACTOR {
    // 0x832AA048 (?ProcessFRAME@aiCACHER@@UAAXXZ) — virtual per-frame tick: refresh the planner's
    // player set (state bit 0x8), then ProcessFrame the watcher subsystem and update the chaser.
    // REVERSED: src/ws/ai/aiCACHER__ProcessFRAME.cpp.
    void ProcessFRAME();
};
