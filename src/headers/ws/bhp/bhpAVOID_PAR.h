#pragma once
#include "../m3d/m3dV.h"
#include "../ds/WEAK_PTR.h"
#include "../ds/dsTSTRING.h"
#include "../ai/aiGRASS.h"   // aiGRASS::GRENADE_INFO (Init source)
#include "bhpPARAMS.h"

// ws-engine bhp subsystem — "avoid grenade/danger" behaviour-parameter block, built on the stack by
// aiBRAIN::implAvoidDanger / implAvoidGrenade and handed to aiBEST_BASE::PushBehaviorByParams.
// DB-verified layout (types_members bhpAVOID_PAR) — size 0x4C: bhpPARAMS base@0 (40B), then the
// avoid-specific fields.

struct entENTITY;             // ent subsystem — game entity (weak-ref target)  boundary (fwd)

struct bhpAVOID_PAR : bhpPARAMS {
    ds::WEAK_PTR<entENTITY> grenade;         // 0x28 weak ref to the grenade being avoided
    m3dV                    posStart;        // 0x2C grenade centre at throw time
    m3dV                    posCDT1st;       // 0x38 first collision-detection / landing point
    float                   timerDelayToStart; // 0x44
    bool                    canFallDown;       // 0x48
    bool                    canRollAwayOnly;   // 0x49
    bool                    canRollAwayAndWait;// 0x4A

    // 0x83278A28 (??0bhpAVOID_PAR@@QAA@XZ) — default-construct an empty avoid param block. boundary.
    bhpAVOID_PAR();
    // 0x832789A8 (??1bhpAVOID_PAR@@UAA@XZ) — virtual destructor in the binary; the vptr is the
    // bhpPARAMS base's explicit __vftable member, so it is declared non-virtual here (project
    // explicit-vptr convention).  boundary.
    ~bhpAVOID_PAR();
    // 0x83279008 (?Init@bhpAVOID_PAR@@QAAXPBUGRENADE_INFO@aiGRASS@@ABV?$dsTSTRING@D@@M@Z) — populate
    // from a tracked grenade, the reaction-list name, and the pre-reaction delay. boundary.
    void Init(const aiGRASS::GRENADE_INFO *grenade, const dsTSTRING<char> &reactions, float delay);
};
