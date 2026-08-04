#pragma once
#include "../ap/apSTATE_T.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsCONST_ARRAY.h"
#include "../ds/dsSTRID_FLAGS.h"
#include "animAF.h"
// ws-engine anim: per-instance action-frame list — tracks which "action frame" events have
// fired/are pending for the animMNG that owns it. DB-verified layout (types_members
// animAF_LIST) — size 420 (0x1A4).

typedef struct animAF_LIST {
    apSTATE_T<unsigned char>            stateAF;              // 0x000
    unsigned char _pad0[3]; /* db-verified padding */
    dsVECTOR<apSTATE_T<unsigned long>, 8> actionFrameStateSeq;// 0x004
    dsCONST_ARRAY<animAF, 31>           afShortList;          // 0x018
    dsSTRID_FLAGS                       events;               // 0x190

    // 0x82C189A8 — clear the AF state, reset the short-list to 31 empty (seqID=-1, extID=-1)
    // entries, and pre-reserve the events flag list to 16 entries.
    animAF_LIST();
} animAF_LIST;
