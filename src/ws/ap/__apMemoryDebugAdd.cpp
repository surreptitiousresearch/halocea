#include "apMEM_BLOCK_TYPE.h"
#include "apMEM_SIZE_STAT.h"
#include "apDLSTAT_SMALL_IFACE.h"
#include "../os/osLOCK.h"
#include "../ds/ds_assert_boundary.h"

// apDlStatFrameNotifyAlloc (this file's sibling, ap_dlstat.cpp) — declared here rather than
// included, since including apMEM_FRAME_DUMPER.h just for this one free-function prototype
// would pull in the whole per-frame dumper type graph.
void apDlStatFrameNotifyAlloc(int size, const char *filename, int line, apMEM_BLOCK_TYPE type);

extern int  _isMemStatSuspend; // boundary global — is memory-stat tracking suspended
extern int  _isMemStatOpened;  // boundary global — has the stat system been opened/started
extern int  isSystemStart;     // boundary global — one-time "system started" latch

osLOCK &apMemStatGetLock();        // boundary — the shared memstat critical section
void apMemoryDebugSystemStart();   // boundary — one-time memory-debug subsystem start
const char *fnmGetNameExt(const char *path); // boundary — filename+ext of a path
void apDumpNoMemLeft();            // boundary — log/report a full debug-stat row

// __apMemoryDebugAdd @ 0x826C0D28
// Record a new live allocation for debug tracking: lazily starts the memory-debug subsystem,
// then (while tracking is enabled/opened and not suspended) buckets the block into
// apDLSTAT_SMALL_IFACE, folds its size into the global apMEM_SIZE_STAT totals, and notifies the
// per-frame dumper. Always returns 1 (the strong-assert path on a full stat row does not abort
// in release builds when IGNORE_STRONG_ASSERT is set).
int __apMemoryDebugAdd(void *memBlock, unsigned int memSize, const char *fileName,
                        unsigned int fileLine, apMEM_BLOCK_TYPE type)
{
    osLOCK &lock = apMemStatGetLock();
    lock.Lock(nullptr, 0);

    if (!_isMemStatSuspend)
    {
        if (!isSystemStart)
        {
            isSystemStart = 1;
            apMemoryDebugSystemStart();
        }

        if (_isMemStatOpened && memBlock)
        {
            const char *nameExt = fileName ? fnmGetNameExt(fileName) : nullptr;

            if (!apDLSTAT_SMALL_IFACE::Add(memBlock, memSize, nameExt, fileLine, type))
            {
                apDumpNoMemLeft();
                if (!IGNORE_STRONG_ASSERT)
                    reinterpret_cast<STRONG_ASSERT_DUMMY *>(0)->Crash(
                        nullptr,
                        "0",
                        "D:\\Projects\\code\\common\\src.sys\\Ap\\ap_dlstat.cpp",
                        205,
                        dsStrongAssertMessage);
            }

            // Deviation: the decompile passes an undefined register ("v13") as apMEM_SIZE_STAT::Add's
            // size argument -- its storage/register for `memSize` was lost across the intervening
            // calls. `memSize` is clearly the intended value (the only live byte-size at this point).
            apMEM_SIZE_STAT &sizeStat = apMEM_SIZE_STAT::Instance();
            sizeStat.Add(memSize, type);

            apDlStatFrameNotifyAlloc(memSize, nameExt, fileLine, type);
        }
    }

    lock.Unlock(nullptr, 0);
    return 1;
}
