#pragma once
#include "../ds/dsVECTOR.h"
// ws-engine anim: collision sub-block of an animTPL.
// DB-verified layout (types_members animTPL_COLL): mopps@0 — size 20.

struct pairMOPP; // Havok MOPP / matrix pairing record (element type; opaque)  boundary

typedef struct animTPL_COLL {
    dsVECTOR<pairMOPP, 8> mopps; // 0x00 per-collidable MOPP records
} animTPL_COLL;
