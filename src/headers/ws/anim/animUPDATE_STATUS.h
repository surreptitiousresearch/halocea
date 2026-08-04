#pragma once
#include "../ap/apSTATE_T.h"
// ws-engine anim: out-status returned by an animation update/command.
// DB-verified layout (types_members animUPDATE_STATUS): stateOut@0 (apSTATE_T<unsigned long>) — size 4.

typedef struct animUPDATE_STATUS {
    apSTATE_T<unsigned long> stateOut; // 0x00 bit0 = transform/pose changed
} animUPDATE_STATUS;
