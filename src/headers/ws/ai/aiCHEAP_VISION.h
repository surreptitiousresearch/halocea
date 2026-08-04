#pragma once
#include "../ds/LIST.h"
#include "aiCHEAVIS.h"

// ws-engine ai08 — the planner's "cheap vision" enemy-visibility cache (aiPLANNER::CheaVis).
// DB-verified layout (types_members aiCHEAP_VISION): __vftable@0, listVIE@4
// (ds::LIST<aiCHEAVIS::VIS_INFO_ENEMY *>) — size 24.

struct aiBRAIN;                  // ai08 — per-unit AI brain   boundary (ptr)
struct entENTITY;                // ent subsystem — game entity boundary (ptr)
struct m3dV;                     // m3d — 3-vector            boundary (ptr/ref)
struct aiCHEAP_VISION_vtbl;      // ai08 — cheap-vision vtable boundary

struct aiCHEAP_VISION {
    aiCHEAP_VISION_vtbl                        *__vftable; // 0x00
    ds::LIST<aiCHEAVIS::VIS_INFO_ENEMY *>       listVIE;   // 0x04 one entry per tracked enemy

    // 0x8323FBE8 (?ProcessFRAME@aiCHEAP_VISION@@QAAXM@Z) — age every enemy's sightings by `delta`.
    // REVERSED.
    void ProcessFRAME(float delta);
    // 0x8323FE30 (?DropAll@aiCHEAP_VISION@@QAAXXZ) — free every VIS_INFO_ENEMY and empty the list.
    // REVERSED.
    void DropAll();
    // 0x8323FE88 (?CalculateViewPercent@aiCHEAP_VISION@@QBAMPBVaiBRAIN@@@Z) — best view-percent of
    // `pBrain`'s current enemy, 0 if none. REVERSED.
    float CalculateViewPercent(const aiBRAIN *pBrain) const;

    // ?NoticeEnemyChanged@aiCHEAP_VISION@@QAAXPBVaiBRAIN@@PBVentENTITY@@1@Z (0x8323FF20) — record
    // that `pBrain`'s tracked enemy changed from `From` to `To`; leaves the old entry and joins (or
    // creates) the new one. REVERSED.
    void NoticeEnemyChanged(const aiBRAIN *pBrain, const entENTITY *From, const entENTITY *To);

    // 0x83240080 (?PostVisibilityInfo@aiCHEAP_VISION@@QAAXPBVaiBRAIN@@ABUm3dV@@M@Z) — forward a fresh
    // sighting of `pSender`'s enemy into that enemy's cache entry. REVERSED.
    void PostVisibilityInfo(const aiBRAIN *pSender, const m3dV &posFrom, float viewPercent);
};
