#pragma once
#include "bhpSMART_PAR.h"

// ws-engine bhp subsystem — turret behavior-parameter block.
// ref-verified layout (headers_ref bhpTURRET_PAR.h) — size 0x40.

struct bhpTURRET_PAR : bhpSMART_PAR {
    bool isCheckForAiming;   // 0x3C

    bhpTURRET_PAR();
};
