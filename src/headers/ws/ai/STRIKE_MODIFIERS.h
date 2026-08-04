#pragma once
#include "../fsm/fsmFLOAT_ARRAY.h"
#include "../m3d/m3dSPL_LINEAR1D.h"
// ws-engine ai08: shared strike-modifier definitions — the aim-point classification and the
// trajectory/sniper tuning blocks used by both wpnS_DESC and aiSTATUS::AIM.
// DB-verified: types_enum_values STRIKE_MODIFIERS::AIM_POINT_TYPE; types_members
// STRIKE_MODIFIERS::PARABOLIC_PARAMS (8), ::THROWER_PARAMS (20), ::SNIPER_PARAMS (20),
// ::ADV_SNIPER_PARAMS (116) — re-checked 2026-08-04.

struct STRIKE_MODIFIERS {
    enum AIM_POINT_TYPE {
        AIM_REGULAR               = 0,
        AIM_SNIPER                = 1,
        AIM_GRENADE               = 2,
        AIM_LAST_VISIBLE_REAL_AIM = 3,
        AIM_ITRC                  = 4,
    };

    struct PARABOLIC_PARAMS {
        float heightMax;       // 0x00 max arc apex height above the line of sight
        bool  isLowTrajectory; // 0x04 pick the low (vs high) parabolic solution
    };

    // DB-verified (types_members STRIKE_MODIFIERS::THROWER_PARAMS) — size 20.
    struct THROWER_PARAMS {
        float distStart;   // 0x00
        float heightStart; // 0x04
        float widthStart;  // 0x08
        float coilsCount;  // 0x0C
        float timeToHit;   // 0x10
    };

    // DB-verified (types_members STRIKE_MODIFIERS::SNIPER_PARAMS) — size 20.
    struct SNIPER_PARAMS {
        float heightSpread; // 0x00
        float widthStart;   // 0x04
        float coilsCount;   // 0x08
        float timeToHit;    // 0x0C
        bool  up;           // 0x10
    };

    // DB-verified (types_members STRIKE_MODIFIERS::ADV_SNIPER_PARAMS) — size 116.
    struct ADV_SNIPER_PARAMS {
        bool           isEnabled;         // 0x00
        bool           isWait;            // 0x01
        unsigned char  _pad02[2];         // 0x02
        fsmFLOAT_ARRAY tgtDistStart;      // 0x04
        m3dSPL_LINEAR1D tgtSpeed;         // 0x10 (16)
        m3dSPL_LINEAR1D tgtBrthSpread;    // 0x24 (36)
        fsmFLOAT_ARRAY tgtBrthPeriod;     // 0x38 (56)
        fsmFLOAT_ARRAY waitTimeOpen;      // 0x44 (68)
        fsmFLOAT_ARRAY waitRadiusMistake; // 0x50 (80)
        fsmFLOAT_ARRAY shotTimeTargeting; // 0x5C (92)
        fsmFLOAT_ARRAY shotTimeWaiting;   // 0x68 (104)
    };
};
