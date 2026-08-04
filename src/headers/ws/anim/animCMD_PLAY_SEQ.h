#pragma once
#include "animCMD_SET_SEQ.h" // animCMD_BASE_SEQ, ANIM_PLAYBACK_TYPE
// ws-engine anim: "play sequence" animation command dispatched through animMNG::SendAnimCmd.
// DB-verified layout (types_members animCMD_PLAY_SEQ): base animCMD_BASE_SEQ@0 (vftable+layerID+
// seqCmd), typeTran@16, playBackType@20, offsetTime@24, blendTime@28, playRate@32 — size 36.

// ANIM_TRAN_TYPE — transition-type selector (unnamed values in the DB). boundary typedef.
typedef int ANIM_TRAN_TYPE;

struct animCMD_PLAY_SEQ_vtbl; // command dispatch table — boundary (slots external to this batch)

struct animCMD_PLAY_SEQ : animCMD_BASE_SEQ {
    ANIM_TRAN_TYPE     typeTran;     // 0x10
    ANIM_PLAYBACK_TYPE playBackType; // 0x14
    float              offsetTime;   // 0x18
    float              blendTime;    // 0x1C
    float              playRate;     // 0x20
};

// `animCMD_PLAY_SEQ::`vftable'' — the command's dispatch table (installed into __vftable when the
// command is built in place). boundary.
extern animCMD_PLAY_SEQ_vtbl animCMD_PLAY_SEQ__vftable;
