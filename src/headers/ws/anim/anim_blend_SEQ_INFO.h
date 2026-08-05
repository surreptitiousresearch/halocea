#pragma once
#include <stdint.h>
#include "../ap/apSTATE_T.h"
// ws-engine anim: blend-layer sequence bookkeeping (namespace anim_blend).
// DB-verified layouts (types_members anim_blend::SEQ_ID_FULL / anim_blend::SEQ_INFO).

struct animSEQ; // animSEQ.h — the referenced sequence (pointer only)  boundary

namespace anim_blend {

    // Fully-qualified sequence identifier. DB size 8 (0x08).
    struct SEQ_ID_FULL {
        int     seqID;  // 0x00  sequence index (-1 = none)
        int16_t extID;  // 0x04  external/bank id
    };

    // Per-instance state of a playing sequence on a blend layer. DB size 40 (0x28).
    struct SEQ_INFO {
        SEQ_ID_FULL              seqID;        // 0x00
        animSEQ                 *pSeq;         // 0x08 resolved sequence
        float                    framePrev;    // 0x0C
        float                    frameCur;     // 0x10
        float                    timeCur;      // 0x14
        float                    playRate;     // 0x18
        float                    playRateBase; // 0x1C
        apSTATE_T<unsigned short> state;        // 0x20

        // 0x82BF8CD8 — a sequence is valid when pSeq is set, seqID != -1, and the sequence
        // itself is loaded (stateSeq & 0x100). Const method.
        int IsValid() const;
    };

}
