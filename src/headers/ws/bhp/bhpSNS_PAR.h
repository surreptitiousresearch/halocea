#pragma once
#include "../m3d/m3dV.h"
#include "bhpSTAY_PAR.h"
#include "../ai/aiRETICLE.h"

// ws-engine bhp subsystem — "stay & notify sound" behaviour-parameter block, built on the stack by
// aiTASK_DATA_FN::AFRestart and handed to aiBEST_BASE::SetBehaviorByParams. DB-verified layout
// (types_members bhpSNS_PAR) — size 104 (0x68): bhpSTAY_PAR base@0 (44B), then the SNS-specific
// fields.

struct bhpSNS_PAR : bhpSTAY_PAR {
    float         timeToExecute;    // 0x2C
    unsigned char termChecker[16];  // 0x30 __int128 (pointer-to-member-function termination checker)
    m3dV          posTermCheck;     // 0x40 termination-check reference position
    aiRETICLE     rtc;              // 0x4C
    m3dV          posToStay;        // 0x58 commanded stay position
    bool          isGivenPosToStay; // 0x64 whether posToStay was explicitly supplied

    // Default-construct an empty stay-&-notify param block. Body external (bhp).  boundary.
    bhpSNS_PAR();
    // Destroy: restore the bhpPARAMS vtable and free the base's failNotifyAcceptorsSNum vector
    // (inlined by the compiler at every call site). Body external (bhp).  boundary.
    ~bhpSNS_PAR();
};
