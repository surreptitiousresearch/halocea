#pragma once
#include <stdint.h>
#include "anim_blend_SEQ_INFO.h"
// ws-engine anim: one "action frame" record — a sequence id paired with the action-frame index
// inside it. DB-verified layout (types_members animAF) — size 9 (padded to 12 in
// dsCONST_ARRAY<animAF,31> storage, per the observed 12-byte element stride).

typedef struct animAF {
    anim_blend::SEQ_ID_FULL seqNmb;        // 0x00
    uint8_t         actionFrameID; // 0x08
} animAF;
