// osCreateThread @0x825E6370 — heap-allocate a {func,param} pair, hand it to Win32 CreateThread
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
    /* The block holds exactly two pointers: the binary asks for a literal 8 (`li r3, 8`
       @0x825E6394) and then stores both of them into it (`stw r28, 0(r3)` @0x825E63A4 = func,
       `stw r27, 4(r3)` @0x825E63A8 = param). 8 is that count times the 4-byte pointer of this
       PPC32 target, so it is sized from the element type here per the x64-portability directive:
       the request is still 8 on PPC32, and the `args[1]` store no longer runs past the end of the
       block where pointers are 8 bytes wide. */
    void **args = (void **)operator new(2 * sizeof(void *), "D:\\Projects\\code\\common\\src.sys\\ap\\ap_os_xenon.cpp", 0x533);
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
