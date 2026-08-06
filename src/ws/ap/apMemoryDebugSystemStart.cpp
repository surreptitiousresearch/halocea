// apMemoryDebugSystemStart @ 0x826C0CA8 — one-time bring-up of the small-allocation memory
// debug statistics system: allocate/publish the _apMemStat hash (apDLSTAT_SMALL_IFACE::Start),
// suspended so the bootstrap allocation is not itself tracked, then register the matching
// shutdown at exit. No-op if already opened.
#include "../../headers/ws/ap/apDLSTAT_SMALL_IFACE.h"
#include <stdlib.h>

extern int _isMemStatOpened;   // boundary — memstat system live flag
extern int _isMemStatSuspend;  // boundary — when set, allocations are not recorded
extern "C" int isMemStopInited;    // boundary — atexit(stop) registration guard

extern void apMemoryDebugSystemStop(); // 0x826C... shutdown counterpart

void apMemoryDebugSystemStart()
{
    if (_isMemStatOpened)
        return;

    _isMemStatSuspend = 1;
    apDLSTAT_SMALL_IFACE::Start();
    _isMemStatSuspend = 0;
    _isMemStatOpened = 1;

    if (!isMemStopInited)
    {
        isMemStopInited = 1;
        atexit(apMemoryDebugSystemStop);
    }
}
