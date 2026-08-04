#pragma once
#include "../ds/WEAK_PTR.h"
#include "../ds/dsSTRID.h"
#include "../m3d/m3dV.h"
#include "../gs/gsGEOM_ANCHOR.h"
// ws-engine dmg subsystem: the descriptor of a damage/shot event's participants and geometry.
// DB-verified layout (types_members dspPARAMS): hitInfo@0 (gsGEOM_ANCHOR, 60B), entSender@60,
// entShooter@64, typeGun@68, typeToolGun@72, orgDmg@76, dirDmg@88, dmgScale@100, radius@104,
// isDirectHit@108, entSenderId@112, entShooterId@116 — total size 120 (0x78).

struct entENTITY;

struct dspPARAMS {
    gsGEOM_ANCHOR           hitInfo;      // 0x00
    ds::WEAK_PTR<entENTITY> entSender;    // 0x3C
    ds::WEAK_PTR<entENTITY> entShooter;   // 0x40
    dsSTRID                 typeGun;      // 0x44
    dsSTRID                 typeToolGun;  // 0x48
    m3dV                    orgDmg;       // 0x4C
    m3dV                    dirDmg;       // 0x58
    float                   dmgScale;     // 0x64
    float                   radius;       // 0x68
    bool                    isDirectHit;  // 0x6C
    int                     entSenderId;  // 0x70
    int                     entShooterId; // 0x74
};
