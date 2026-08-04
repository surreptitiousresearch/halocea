#pragma once
#include "../ds/dsVECTOR.h"
#include "../ai/aiWATCHER_BRAIN_boundaries.h"   // aiBID

// ws-engine bhp subsystem — behavior-parameter base block.
// DB/ref-verified layout (headers_ref bhpPARAMS.h) — size 0x28.

struct bhpPARAMS_vtbl;

struct bhpPARAMS {
    bhpPARAMS_vtbl *__vftable;                       // 0x00
    aiBID           typeBehav;                       // 0x04
    int             isSubBehav;                      // 0x08
    dsVECTOR<unsigned long, 8> failNotifyAcceptorsSNum; // 0x0C
    bool            notifyTaskSys;                    // 0x20
    unsigned int    parentSNumNotifyTS;              // 0x24
};
