#pragma once
// hcexCINE_INFO — per-mask cinematic actor binding. DB-verified layout (size 8):
//   actor@0x00 (gsANITEC_ACTOR* — opaque boundary),
//   mask @0x04 (apSTATE_T<unsigned char> — per-difficulty bitmask).
// Mirrors the definition in src/headers/hcex/hcex_cine_tick_boundary.h; kept as a minimal
// self-contained element header for the dsVECTOR<hcexCINE_INFO,8> instantiations.

#include "../ap/apSTATE_T.h"

struct gsANITEC_ACTOR; // boundary — cinematic actor runtime record

typedef struct hcexCINE_INFO {
    struct gsANITEC_ACTOR   *actor; // 0x00
    apSTATE_T<unsigned char> mask;  // 0x04 per-difficulty bitmask
} hcexCINE_INFO; // 8 bytes (DB-verified)
