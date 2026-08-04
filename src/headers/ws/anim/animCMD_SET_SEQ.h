#pragma once
#include "animCMD_BASE.h"
#include "anim_blend_SEQ_INFO.h"
// ws-engine anim: "set sequence" animation command. Derives animCMD_BASE via animCMD_BASE_SEQ.
// DB-verified layout (types_members animCMD_SET_SEQ / animCMD_BASE_SEQ):
//   animCMD_BASE@0 (vftable+layerID), seqCmd@8 (SEQ_ID_FULL), playBackType@16, offsetTime@20 — size 24.

// ANIM_PLAYBACK_TYPE — DB types_enum_values carries the names (earlier "unnamed" note was
// wrong). CLAMP = play once and hold; CYCLE = loop (the create-data path's ONCE/CYCLED).
enum ANIM_PLAYBACK_TYPE {
    ANIM_PB_DEFAULT = 0,
    ANIM_PB_CLAMP   = 1,
    ANIM_PB_CYCLE   = 2,
    ANIM_PB_MANUAL  = 3,
    ANIM_PB_REPLAY  = 4,
    ANIM_PB_RESTART = 5,
};

// animCMD_BASE_SEQ = animCMD_BASE + a fully-qualified sequence id.
struct animCMD_BASE_SEQ : animCMD_BASE {
    anim_blend::SEQ_ID_FULL seqCmd; // 0x08
};

struct animCMD_SET_SEQ : animCMD_BASE_SEQ {
    ANIM_PLAYBACK_TYPE playBackType; // 0x10
    float              offsetTime;   // 0x14
};
