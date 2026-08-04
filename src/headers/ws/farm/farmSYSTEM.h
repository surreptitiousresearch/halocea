#pragma once
#include "../ia/iaIACTOR.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "farmUNIT.h"
#include "farmGROUP.h"
// ws-engine farm subsystem manager (farmSYSTEM). Full DB-verified layout (types_members
// farmSYSTEM, reconciled 2026-08-04): iaIACTOR base (200B) + members below — size 292 (0x124).
// (Distinct from the reconstructed `farm_system` in src/headers/farm_system.h, whose adjudicated
// minimal layout put the flag at 0x0 -- this is the address-verified farmSYSTEM class layout.)

struct farmSYSTEM : iaIACTOR {
    int  visChangedFrame;                     // 0x0C8 frame index of the last visibility change
    int  isFirstFrame;                        // 0x0CC
    int  isForceUpdate;                       // 0x0D0
    int  isUnfarmingAfterVisChange;           // 0x0D4
    dsVECTOR<farmUNIT *, 8> unitList;         // 0x0D8
    dsVECTOR<farmUNIT, 8>   unitListExplicit; // 0x0EC
    dsTSTRING<char>         nameCur;          // 0x100
    dsTSTRING<char>         nameClassCur;     // 0x104
    dsTSTRING<char>         nameTplCur;       // 0x108
    bool isSuspendAdd;                        // 0x10C set while additions to the farm are suspended
    bool processWOVisDomains;                 // 0x10D
    unsigned char _pad10E[2];                 // 0x10E db-verified padding
    dsVECTOR<farmGROUP, 8>  groupList;        // 0x110
    // 292 bytes

    // ?IsSuspendAdd@farmSYSTEM@@QBA_NXZ @ 0x823B08F0 -- const getter for isSuspendAdd.
    bool IsSuspendAdd() const;
};
