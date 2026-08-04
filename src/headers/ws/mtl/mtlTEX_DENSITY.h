#pragma once
#include "../ds/dsARRAY.h"
// ws-engine mtl: per-mtlTBL texel-density table. DB-verified layout (types_members
// mtlTEX_DENSITY): u@0 (ds::ARRAY<float,5>), v@20 (ds::ARRAY<float,5>) — size 40.
// mtlTBL::Init resets every u[i]/v[i] to 1.0.

typedef struct mtlTEX_DENSITY {
    ds::ARRAY<float, 5> u; // 0x00
    ds::ARRAY<float, 5> v; // 0x14
} mtlTEX_DENSITY;
