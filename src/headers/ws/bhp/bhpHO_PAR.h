#pragma once
#include "bhpPARAMS.h"
#include "../m3d/m3dV.h"
#include "../ai/aiRETICLE.h"

// ws-engine bhp subsystem — hideout ("HO") behavior-parameter block. DB-verified layout
// (types_members bhpHO_PAR) — size 0x68: bhpPARAMS base@0 (40B), then the HO-specific fields.
// termChecker is a pointer-to-member selector (DB member type __int128, 16B) kept byte-accurate.

struct bhpHO_PAR : bhpPARAMS {
    short         wpid;                 // 0x28 destination HO waypoint id
    // 2 bytes padding to 0x2C
    unsigned int  flagsBlockedWP;       // 0x2C
    bool          allowAcquireHO;       // 0x30
    bool          isScanWP;             // 0x31
    // 2 bytes padding to 0x34
    float         timeToExecute;        // 0x34
    unsigned char termChecker[0x10];    // 0x38 __int128 pointer-to-member selector (opaque)
    m3dV          penaltyCheckPos;      // 0x48
    float         distEnemyFar;         // 0x54
    bool          validateVsAllEnemies; // 0x58
    // 3 bytes padding to 0x5C
    aiRETICLE     rtc;                  // 0x5C

    bhpHO_PAR();   // boundary ctor
    ~bhpHO_PAR();  // boundary dtor
};
