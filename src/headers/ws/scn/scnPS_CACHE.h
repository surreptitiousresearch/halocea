#pragma once
#include "../ds/dsVECTOR.h"
// ws-engine scn: parsed-section cache. DB-verified layout (types_members scnPS_CACHE):
// data@0 (dsVECTOR<scnPS_CACHE::REC,8>, 20B) — size 20.

typedef struct scnPS_CACHE {
    struct REC; // boundary — cache record (vector element)
    dsVECTOR<REC, 8> data; /* 0x00 */
} scnPS_CACHE;
