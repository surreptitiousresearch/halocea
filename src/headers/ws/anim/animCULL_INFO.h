#pragma once
#include <stdint.h>
// ws-engine anim: per-instance visibility/cull cache. DB-verified layout
// (types_members animCULL_INFO) — size 52 (0x34).

typedef struct animCULL_INFO {
    int64_t      camMask;      // 0x00 camera visibility mask (DB: __int64; note the embedding
                               //      in animINST at 0x1BC is only 4-aligned)
    int          isValid;      // 0x08
    float        camDistList[10]; // 0x0C per-camera distance cache
} animCULL_INFO;
