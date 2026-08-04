#pragma once
#include "../ds/LIST.h"
#include "../m3d/m3dV.h"

// ws-engine ai08 (aiCHEAVIS namespace) — the "cheap vision" per-enemy visibility cache records.
// DB-verified layouts (types_members aiCHEAVIS::VIS_INFO / aiCHEAVIS::VIS_INFO_ENEMY). Each tracked
// enemy owns one VIS_INFO_ENEMY holding a list of per-sender VIS_INFO sightings.

struct aiBRAIN; // ai08 — per-unit AI brain   boundary (ptr)

namespace aiCHEAVIS {

// One sighting of the tracked enemy by a single sender. DB-verified layout, size 24.
struct VIS_INFO {
    float              timerToDelete; // 0x00 seconds until this stale sighting is dropped
    float              viewPercent;   // 0x04 how visible the enemy is from posFrom
    m3dV               posFrom;       // 0x08 sample origin the sighting was taken from
    const unsigned int mpIDSender;    // 0x14 uniqueID of the observing sender
};

// Per-enemy cheap-vision aggregate: observer/attacker counts plus the list of sightings.
// DB-verified layout, size 32.
struct VIS_INFO_ENEMY {
    unsigned int       mpIDEnemy;      // 0x00 uniqueID of the tracked enemy
    int                ObserversCount; // 0x04
    int                AttackersCount; // 0x08
    ds::LIST<VIS_INFO> listVI;         // 0x0C per-sender sightings

    // 0x8323F810 (??0VIS_INFO_ENEMY@aiCHEAVIS@@QAA@K@Z) — construct empty for `mpIDEnemy`. boundary.
    VIS_INFO_ENEMY(unsigned int mpIDEnemy);
    // 0x8323F808 — destroy (clears listVI). boundary.
    ~VIS_INFO_ENEMY();

    // 0x8323F280 — the tracked enemy's uniqueID. boundary.
    unsigned int GetMPID() const;
    // 0x8323F850 — age every sighting by `delta`, dropping expired ones. boundary.
    void ProcessFrame(float delta);
    // 0x8323FA28 — `pBrain` stops observing this enemy (observer/attacker bookkeeping). boundary.
    void Leave(const aiBRAIN *pBrain);
    // 0x8323FB20 — best current view-percent of this enemy as seen for `pBrain`. boundary.
    float CalculateViewPercent(const aiBRAIN *pBrain) const;
    // 0x8323FCC8 — `pBrain` starts observing this enemy. boundary.
    void Join(const aiBRAIN *pBrain);
    // 0x8323FD60 — record a fresh sighting from `mpIDSender` at `posFrom` with `viewPercent`.
    // boundary.
    void PostVisibilityInfo(unsigned int mpIDSender, const m3dV &posFrom, float viewPercent);
};

} // namespace aiCHEAVIS
