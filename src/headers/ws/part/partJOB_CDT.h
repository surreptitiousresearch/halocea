#pragma once
#include "../jbm/jbmJOB.h"
#include "../ds/dsCONST_ARRAY.h"
// ws-engine part: particle-system collide-detect-transform job dispatcher. Out of scope beyond
// the one entry point hcexPARTICLE_SYS::PushDelayedCDT calls; bodies external to this batch.
// DB-verified layout (types_members partJOB_CDT): jbmJOB (base)@0 (12B),
// stopJob@12 (dsCONST_ARRAY<int,8>, 36B) — size 48.

struct partJOB_CDT : jbmJOB {
    dsCONST_ARRAY<int, 8> stopJob; // 0x0C

    // 0x82673A30 (DB ?Start@partJOB_CDT@@QAAXH@Z; an earlier note said 0x8290ECB0, which is
    // mid-body of an unrelated dsVECTOR::PushBack) — kick the CDT job for thread `threadID`
    // (2/4/16 = the three CDT thread masks
    // hcexPARTICLE_SYS::PushDelayedCDT dispatches after the base gsPARTICLE_SYS pass).
    void Start(int threadID); // boundary
    void Stop(int threadID);  // boundary — halt the CDT job on the given worker thread mask
};

extern partJOB_CDT partJobCdt; // the single global CDT job instance
