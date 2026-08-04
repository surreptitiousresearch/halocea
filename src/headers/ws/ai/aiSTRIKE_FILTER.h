#pragma once
#include "../m3d/m3dV.h"
#include "../ds/dsSTRID_FLAGS.h"
// ws-engine ai08: a query filter describing which strike/attack options an AI may consider when
// choosing how to engage. DB-verified layout (types_members aiSTRIKE_FILTER) — size 0x54.

// DB types_enum_values(aiFILTER_STRIKE_VALUE): per-option ternary (ignore / allow / require-only).
enum aiFILTER_STRIKE_VALUE {
    fvNO   = 0,
    fvYES  = 1,
    fvONLY = 2,
};

// DB types_enum_values(aiFILTER_POS_VALUE): how the position constraint is interpreted.
enum aiFILTER_POS_VALUE {
    fvIGNORE  = 0,
    fvCURRENT = 1,
    fvGIVEN   = 2,
};

struct aiSTRIKE_FILTER {
    aiFILTER_STRIKE_VALUE qus;             // 0x00
    aiFILTER_STRIKE_VALUE grenades;        // 0x04
    aiFILTER_STRIKE_VALUE melee;           // 0x08
    aiFILTER_STRIKE_VALUE blind;           // 0x0C
    aiFILTER_STRIKE_VALUE covered;         // 0x10
    aiFILTER_STRIKE_VALUE turret;          // 0x14
    aiFILTER_POS_VALUE    fvPos;           // 0x18
    m3dV                  pos;             // 0x1C
    bool                  isFillCutOff;    // 0x28
    bool                  isForbidStayOnly; // 0x29
    bool                  checkPaths;      // 0x2A
    bool                  checkIsAimed;    // 0x2B
    bool                  checkSparsing;   // 0x2C
    unsigned char         _pad2D[3];       // 0x2D
    aiFILTER_STRIKE_VALUE hi_grav;         // 0x30
    aiFILTER_STRIKE_VALUE low_grav;        // 0x34
    aiFILTER_STRIKE_VALUE manip;           // 0x38
    bool                  isForcedClear;   // 0x3C
    unsigned char         _pad3D[3];       // 0x3D
    dsSTRID_FLAGS         flags;           // 0x40 (size 0x14; flags.propList.pData is heap-freed)

    // 0x8317xxxx (?aiSTRIKE_FILTER@...ctor) — default-initialise all filter options. boundary body.
    aiSTRIKE_FILTER();
};
