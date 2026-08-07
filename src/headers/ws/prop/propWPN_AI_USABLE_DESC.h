#pragma once
#include "../m3d/m3dV.h"
#include "../ds/dsTYPE_ID.h"
#include "../wpn/aimCHECK_TYPE.h"
// ws-engine prop: the shared descriptor image behind a propWPN_AI_USABLE (its propBASE::spDesc
// pointee). Full layout belongs to the prop/wpn re-source frontier; only the fields the ai08 weapon
// module reads are typed here, at their disassembly-verified offsets (a8_weapon.cpp @0x83294FE0
// reads +0x84; @0x83294AE8 reads the +0x8C..+0x94 tip-offset triple). The remainder stays opaque.

struct propWPN_AI_USABLE_DESC {
    unsigned char _pad0[0x84];       // 0x000 propUSABLE_DESC base + fields (frontier, opaque)
    aimCHECK_TYPE typeIsAimed;       // 0x084 default aim-check type for this controller
    unsigned char _pad88[4];         // 0x088
    m3dV          tipOffset;         // 0x08C body-local muzzle-tip offset (DB: shiftPosTip)
                                     //       (x -> dirUBodyRight, y -> dirUpBody, z -> dirUBody)

    // DB-verified static (?TYPE_ID@propWPN_AI_USABLE_DESC@@2VdsTYPE_ID@@A) — this class's runtime
    // type id, used by iaIACTOR property lookups.
    static dsTYPE_ID TYPE_ID;
};
