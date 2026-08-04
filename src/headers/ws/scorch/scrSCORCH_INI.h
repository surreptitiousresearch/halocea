#pragma once
// ws-engine scorch/decal: descriptor passed to scrFAMILY::Add to spawn one decal/scorch mark.
// DB-verified layout (see hcex_create_decals_delayed_boundary.h, verified via types_members) —
// size 84. Derives from msgDATA (message-queue header word) so a scorch request can travel through
// the ws msg system.
#include "../msg/msgDATA.h"
#include "../ds/dsTSTRING.h"
#include "../cdt/cdtINFO.h"
#include "../m3d/m3dV.h"

struct atkDISPATCHER; // atk subsystem — attack dispatcher (pointer only)

typedef struct scrSCORCH_INI
{
    msgDATA             base;         // 0x00 (anonymous msgDATA base: sign, sizeStruct)
    dsTSTRING<char>     nameClass;    // 0x04 scorch-family class name
    cdtINFO             cdtInfo;      // 0x08 (36B) collision/surface info
    float               alphaScale;   // 0x2C
    float               radScale;     // 0x30
    float               timeAdded;    // 0x34
    float               radius;       // 0x38
    float               rotAngle;     // 0x3C
    int                 texIdx;       // 0x40
    m3dV                normal;       // 0x44
    const atkDISPATCHER *pAtkDisp;    // 0x50

    // 0x827454A8 — default ctor: tag msgDATA header (sign=64), adopt empty nameClass, zero-init
    // cdtInfo, default scales/radius to 1, and finally stamp sizeStruct=84.
    scrSCORCH_INI();
    // 0x82C95D4C — copy ctor from an existing ini (used by scrFAMILY::Add). boundary.
    scrSCORCH_INI(const scrSCORCH_INI &that);
} scrSCORCH_INI;                       // 84 bytes
