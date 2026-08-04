#pragma once
#include "mdlITRC_BASE.h"
#include "../cdt/cdtRIDE_INFO.h"
#include "../cdt/cdtACCEPT_MOVE_CHECKER.h"
#include "../m3d/m3dV.h"
// ws-engine mdl: the hideout (HO) itrc — a runtime handle onto a hideout attach point.
// DB-verified (types_members mdlITRC_HO, re-checked 2026-08-04), size 0x17C.

struct gmHO_CTRL_BASE;

struct mdlITRC_HO : mdlITRC_BASE {
    // DB-verified values (types_enum_values mdlITRC_HO::TYPE_ORIENT).
    enum TYPE_ORIENT {
        ORIENT_UNDEF = 0,
        ORIENT_LEFT  = 1,
        ORIENT_RIGHT = 2,
    };
    // DB-verified values (types_enum_values mdlITRC_HO::MINIZOOM_CORNER_STATE).
    enum MINIZOOM_CORNER_STATE {
        MINIZOOM_CORNER_UNKNOWN = 0,
        MINIZOOM_CORNER_NONE    = 1,
        MINIZOOM_CORNER_RIGHT   = 2,
        MINIZOOM_CORNER_LEFT    = 3,
    };

    gmHO_CTRL_BASE        *hoCtrl;                // 0x08
    bool                   isValid;               // 0x0C
    bool                   isSnapped;             // 0x0D
    bool                   isSpecialUpdate;       // 0x0E
    bool                   disableAimPosSnapping; // 0x0F
    bool                   isLocked;              // 0x10
    unsigned char          _pad11[3];             // 0x11
    TYPE_ORIENT            orient;                // 0x14 (20)
    bool                   isInner;               // 0x18 (24)
    unsigned char          _pad19[3];             // 0x19
    float                  snapDist;              // 0x1C (28)
    bool                   wasReached;            // 0x20 (32)
    unsigned char          _pad21[3];             // 0x21
    float                  elapsedTime;           // 0x24 (36)
    m3dV                   additionalOffsetGoal;  // 0x28 (40)
    m3dV                   additionalOffsetCur;   // 0x34 (52)
    float                  timeLeftToReach;       // 0x40 (64)
    float                  timeOfCreation;        // 0x44 (68)
    float                  timeSpecialUpdate;     // 0x48 (72)
    bool                   canBeLocked;           // 0x4C (76)
    unsigned char          _pad4D[3];             // 0x4D
    m3dV                   vUp;                   // 0x50 (80)
    cdtRIDE_INFO           hoRideWatcher;         // 0x5C (92, 252B)
    cdtACCEPT_MOVE_CHECKER moveCheckerRight;      // 0x158 (344)
    cdtACCEPT_MOVE_CHECKER moveCheckerLeft;       // 0x168 (360)
    MINIZOOM_CORNER_STATE  minizoomCornetState;   // 0x178 (376, DB spelling)
};                                                // 0x17C (380) bytes
