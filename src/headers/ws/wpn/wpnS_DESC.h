#pragma once
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"
#include "../fsm/fsmFLOAT_ARRAY.h"
#include "../ai/STRIKE_MODIFIERS.h"
#include "../ds/dsFLAGS_CONDITION.h"
#include "../ds/dsSTRID_FLAGS.h"
#include "../ctrl/ctrlPROP_ID.h"
#include "../ctrl/ctrlPROP_LIST.h"

// DB-verified values (types_enum_values wpnTRAJ_TYPE).
enum wpnTRAJ_TYPE { trajLINEAR = 0, trajSPLASH = 1, trajCUSTOM = 2 };

// ws-engine wpn: weapon-strike descriptor — the tuning block that drives how an AI uses a strike
// weapon. DB-verified layout (types_members wpnS_DESC, re-checked 2026-08-04), total size 416;
// all aggregate members carry their canonical DB types.
struct wpnS_DESC {
    // DB-verified values (types_enum_values wpnS_DESC::SPARSE_TYPE).
    enum SPARSE_TYPE {
        SPARSE_DEFAULT = 0,
        SPARSE_BY_CTRL = 1,
        SPARSE_BY_SUID = 2,
    };

    // DB nested param blocks — each a single STRIKE_MODIFIERS base, no added data
    // (types_members wpnS_DESC::THROWER_PARAMS/SNIPER_PARAMS/ADV_SNIPER_PARAMS).
    struct THROWER_PARAMS    : STRIKE_MODIFIERS::THROWER_PARAMS {};
    struct SNIPER_PARAMS     : STRIKE_MODIFIERS::SNIPER_PARAMS {};
    struct ADV_SNIPER_PARAMS : STRIKE_MODIFIERS::ADV_SNIPER_PARAMS {};

    int            atkID;                          // 0x000
    dsSTRID        eventCtrl;                       // 0x004
    dsSTRID        nameStrike;                       // 0x008
    fsmFLOAT_ARRAY timeShooting;                    // 0x00C
    fsmFLOAT_ARRAY timeThrow;                       // 0x018
    fsmFLOAT_ARRAY timeReadyNext;                   // 0x024
    int            smlStateId;                      // 0x030
    dsTSTRING<char> smlState;                       // 0x034
    fsmFLOAT_ARRAY timeSparse;                      // 0x038
    SPARSE_TYPE    typeSparse;                       // 0x044
    int            typeIsAimed;                      // 0x048 aimCHECK_TYPE (enum) — boundary
    float          distMinUsing;                    // 0x04C
    float          distMaxUsing;                    // 0x050
    float          heightDiffMax;                   // 0x054
    float          coeffAim;                         // 0x058
    float          coeffExagPredict;                // 0x05C
    float          angleMaxLeft;                     // 0x060
    float          angleMaxRight;                    // 0x064
    float          loadPerformance;                 // 0x068
    float          timeSparseOver;                   // 0x06C
    dsTSTRING<char> nameStrikeForOverSparse;         // 0x070
    dsFLAGS_CONDITION<ctrlPROP_ID,ctrlPROP_LIST> condition; // 0x074 unit-property precondition
    dsFLAGS_CONDITION<dsSTRID,dsSTRID_FLAGS> filter;  // 0x08C strike-target property filter
    dsFLAGS_CONDITION<dsSTRID,dsSTRID_FLAGS> acceptVolumes; // 0x0A4 accepted-volume filter
    fsmFLOAT_ARRAY timeForAiming;                    // 0x0BC
    STRIKE_MODIFIERS::AIM_POINT_TYPE typeAimPos;      // 0x0C8
    bool           isNeedToFreezeAim;                // 0x0CC
    float          timerToFreezeAim;                 // 0x0D0
    float          shiftAimH;                         // 0x0D4
    float          BRUTE_UGLY_HACK;                   // 0x0D8
    THROWER_PARAMS paramsThrower;                     // 0x0DC (20)
    SNIPER_PARAMS  paramsSniper;                      // 0x0F0 (20)
    ADV_SNIPER_PARAMS paramsAdvSniper;                // 0x104 (116)
    STRIKE_MODIFIERS::PARABOLIC_PARAMS paramsParabolic; // 0x178 (8)
    wpnTRAJ_TYPE   typeTraj;                           // 0x180
    bool           isFreeMovement;                    // 0x184
    bool           isStayOnly;                         // 0x185
    bool           isUseLaser;                          // 0x186
    bool           needShootPathCheck;                // 0x187
    bool           avoidShootingFriends;              // 0x188
    bool           isAllowHang;                        // 0x189
    bool           isWeaponDirByBody;                 // 0x18A
    bool           isFavouriteWhenEnemyCovered;       // 0x18B
    bool           canShootAtEnemyHitByGrav;          // 0x18C
    bool           canShootAtEnemyIn0G;               // 0x18D
    bool           canShootAtEnemyHitByHiGrav;        // 0x18E
    bool           canShootAtPlayer;                  // 0x18F
    bool           isEnabledIn1G;                     // 0x190
    bool           isEnabledIn0G;                     // 0x191
    bool           isEnabledIn0gShoot;                // 0x192
    bool           isEnabledWhenPumped;               // 0x193
    bool           isWhenPumpedOnly;                  // 0x194
    bool           isEnabledIn0gHO;                   // 0x195
    bool           isSkipBodyChecks;                  // 0x196
    bool           isOKWithoutSpeed;                  // 0x197
    bool           mayUseOnPath;                      // 0x198
    // 3 bytes tail padding to align SUID at 0x19C (compiler-inserted).
    int            SUID;                              // 0x19C weapon strike unique id

    // ?MELEE@wpnS_DESC@@2VdsSTRID@@B @ 0x84323C80 — interned strike-property name identifying a
    // melee strike (matched against a descriptor's filter). static const.
    static const dsSTRID MELEE;
};
