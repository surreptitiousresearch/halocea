#pragma once
#include "../ap/apSTATE_T.h"
// ws-engine anim: per-step update descriptor passed to animMNG::UpdateAnim / animINST::UpdateAnimation.
// DB-verified layout (types_members animMNG_UPDATE_INFO): dt@0, playRate@4, state@8 — size 12.

typedef struct animMNG_UPDATE_INFO {
    float                    dt;       // 0x00 elapsed time for this step
    float                    playRate; // 0x04 global playback-rate multiplier
    apSTATE_T<unsigned long> state;    // 0x08 update flags
} animMNG_UPDATE_INFO;
