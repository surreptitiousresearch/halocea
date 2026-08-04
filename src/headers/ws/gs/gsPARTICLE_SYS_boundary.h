#pragma once
// Boundary declarations for gsPARTICLE_SYS::PushDelayedCDT / StopDelayedCDT (0x82677AF0 /
// 0x82673A98). These methods operate over file-scope particle-CDT globals and kick the single
// global partJOB_CDT job; the referenced types/functions below are engine internals reached
// through these two methods and are kept as extern boundary decls for this batch.

#include "../part/partJOB_CDT.h"   // partJOB_CDT + `extern partJOB_CDT partJobCdt`
#include "../cdt/cdtINFO.h"        // cdtINFO result element type
#include "../ds/dsVECTOR.h"        // dsVECTOR<cdtINFO,8> (Resize)

// Pending collision-detection request id, clamped to 0xFFFF each push.
extern int partCdtCurId;
// Result buffer sized to the pending request count before the job runs.
extern dsVECTOR<cdtINFO, 8> partCdtResults;
// Multi-thread configuration selector (2 -> 4 masks, 4 -> 32 masks, else single-threaded).
extern int gsMsgMTConfig;

// PIX profiler scope markers — platform/boundary.
extern void osPIXBeginEvent(const char *name);
extern void osPIXEndEvent();
