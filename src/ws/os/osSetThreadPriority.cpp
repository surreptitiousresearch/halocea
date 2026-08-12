// osSetThreadPriority @0x825E6430 — map the engine's 0..6 priority enum to a Win32
// THREAD_PRIORITY_* value and apply it via SetThreadPriority. STRONG_ASSERT(0) on an
// out-of-range priority (>6) rather than clamping.
#include "../../headers/ws/os/osHANDLE_DUMMY.h"
#include "../../headers/ws/os/OS_THREAD_PRIORITY.h"

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h" // the only Crash is a C++ member, not a C symbol
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */
extern "C" void SetThreadPriority(osHANDLE_DUMMY *thread, int priority);

void osSetThreadPriority(osHANDLE_DUMMY *ptrThread, unsigned int prior)
{
    int win32Priority;

    if (prior > OS_THREAD_PRIORITY_TIME_CRITICAL) {
        if (!IGNORE_STRONG_ASSERT)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("0", "D:\\Projects\\code\\common\\src.sys\\ap\\ap_os_xenon.cpp", 1375, empty_string);
        win32Priority = 0; // DEVIATION: decompiler leaves this branch's value uninitialized
                            // (reads an uninitialized stack slot); 0 (NORMAL) substituted here.
    } else {
        switch (prior) {
            case OS_THREAD_PRIORITY_LOWEST:       win32Priority = -2; break; // THREAD_PRIORITY_LOWEST
            case OS_THREAD_PRIORITY_BELOW_NORMAL: win32Priority = -1; break; // THREAD_PRIORITY_BELOW_NORMAL
            case OS_THREAD_PRIORITY_NORMAL:       win32Priority = 0;  break; // THREAD_PRIORITY_NORMAL
            case OS_THREAD_PRIORITY_ABOVE_NORMAL: win32Priority = 1;  break; // THREAD_PRIORITY_ABOVE_NORMAL
            case OS_THREAD_PRIORITY_HIGHEST:      win32Priority = 2;  break; // THREAD_PRIORITY_HIGHEST
            default: win32Priority = prior ? 15 : -15; break; // IDLE -> -15, TIME_CRITICAL -> 15
        }
    }

    SetThreadPriority(ptrThread, win32Priority);
}
