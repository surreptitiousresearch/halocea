// osCreateThread @0x825E63A0 — heap-allocate a {func,param} pair, hand it to Win32 CreateThread
// via the fixed osCreateThread_DummyProc trampoline (which unpacks and calls func(param)),
// STRONG_ASSERT the handle came back non-null, name the thread, and return the handle.
#include "../../headers/ws/os/osHANDLE_DUMMY.h"
#include <new>

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h" // the only Crash is a C++ member, not a C symbol
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */
extern "C" void *CreateThread(void *secAttrs, unsigned int stackSize, void *startRoutine, void *param, unsigned int flags, unsigned int *outThreadId);
extern "C" void  SetThreadName(unsigned int threadId, const char *name);
extern "C" void  osCreateThread_DummyProc(void *args);
// Debug placement operator new the ws build links in. boundary.
void *operator new(size_t size, const char *file, unsigned int line);

osHANDLE_DUMMY *osCreateThread(void (*func)(void *), void *param, const char *name, int stackSize)
{
    void **args = (void **)operator new(8, "D:\\Projects\\code\\common\\src.sys\\ap\\ap_os_xenon.cpp", 0x533);
    args[0] = (void *)func;
    args[1] = param;

    unsigned int threadId = 0;
    void *handle = CreateThread(nullptr, stackSize == -1 ? 0 : (unsigned int)stackSize,
                                 (void *)&osCreateThread_DummyProc, args, 0, &threadId);
    if (!IGNORE_STRONG_ASSERT && !handle)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("handle != 0", "D:\\Projects\\code\\common\\src.sys\\ap\\ap_os_xenon.cpp", 1341, empty_string);

    SetThreadName(threadId, name);
    return (osHANDLE_DUMMY *)handle;
}
