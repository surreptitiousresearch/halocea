#pragma once
// HCEX sound bridge: per-call 3D/2D playback properties passed into FindSoundByName /
// PlayCustomSound call sites (position, cone, distance-to-listener, etc).
// DB-verified layout (types_members HALO_CHANNEL_PROPERTIES): volume@0, pitch@4,
// underwater@8, is3D@9, minDist@12, maxDist@16, distanceToPlayer@20, innerConeAngle@24,
// outerConeAngle@28, outerConeGain@32, position@36 (m3dV), forward@48 (m3dV),
// velocity@60 (m3dV), occlusion@72, obstruction@76 — size 80.
// Sibling type: owned by the parallel HALO_CHANNEL drain; declared here from the DB only far
// enough to type HALO_SOUND_SYSTEM's field reads.

#include "../ws/m3d/m3dV.h"

typedef struct HALO_CHANNEL_PROPERTIES {
    float volume;            // 0x00
    float pitch;              // 0x04
    bool  underwater;         // 0x08
    bool  is3D;                // 0x09
    unsigned char _pad0[2]; /* db-verified padding */
    float minDist;             // 0x0C
    float maxDist;             // 0x10
    float distanceToPlayer;    // 0x14
    float innerConeAngle;      // 0x18
    float outerConeAngle;      // 0x1C
    float outerConeGain;       // 0x20
    m3dV  position;            // 0x24
    m3dV  forward;             // 0x30
    m3dV  velocity;            // 0x3C
    float occlusion;           // 0x48
    float obstruction;         // 0x4C

    HALO_CHANNEL_PROPERTIES(); // ??0HALO_CHANNEL_PROPERTIES@@QAA@XZ -- zero-fills the whole struct
} HALO_CHANNEL_PROPERTIES;
