#pragma once
#include "../os/osLOCK.h"
// ws-engine vid: video-driver critical-section lock. Wraps an osLOCK and layers
// recursive-lock bookkeeping (lockDepth/lockThread) plus a one-shot call into the
// vidDRIVER singleton's thread-ownership hooks on the outermost Lock/Unlock.
// DB-verified layout (types_members vidLOCK): lock@0 (osLOCK, 52B), lockDepth@52,
// lockThread@56 — size 60.

typedef struct vidLOCK {
    osLOCK lock;       // 0x00 — underlying platform critical section
    int    lockDepth;  // 0x34 — vidLOCK's own recursion count (distinct from lock.lockDepth)
    int    lockThread; // 0x38 — id of the thread currently holding the outer lock

    // 0x82667738 — acquire the underlying osLOCK, then track vidLOCK's own recursion
    // depth/owning thread; on the outermost acquire (lockDepth becomes 1), notify the
    // vidDRIVER singleton so it can claim thread ownership of the video device.
    void Lock(const char *file, int line);

    // 0x826677F0 — mirror of Lock: on the outermost release (lockDepth was 1), release
    // vidDRIVER's thread ownership and clear lockThread, then decrement lockDepth and
    // release the underlying osLOCK.
    void Unlock(const char *file, int line);
} vidLOCK;
