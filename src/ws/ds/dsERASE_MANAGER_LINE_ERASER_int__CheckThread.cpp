#include "dsERASE_MANAGER.h"
#include "dsLF_SORTED_MAP.h"
#include "ds_assert_boundary.h"

struct dsSTR_HASH; // boundary — string hash policy
struct dsSTR_CMP;  // boundary — string compare policy

// Current lock-free-container thread ordinal [0,32). // boundary
int dsERASE_MANAGER_LINE_ERASER_int__GetThreadId();   /* dsERASE_MANAGER<LINE_ERASER<int>>::GetThreadId @0x82613B18; disambiguated from the dsSTATE_MGR twin */

typedef dsLF_SORTED_MAP<const char *, int, dsSTR_HASH, dsSTR_CMP>::LINE_ERASER StrIntLineEraser;

// dsERASE_MANAGER<dsLF_SORTED_MAP<char const*,int,dsSTR_HASH,dsSTR_CMP>::LINE_ERASER>::CheckThread
// @ 0x82613E80 — debug thread-affinity guard: assert the calling thread has a valid id and
// already owns an allocation bit in this manager before it traverses the lock-free container.
template<>
void dsERASE_MANAGER<StrIntLineEraser>::CheckThread()
{
    if (IGNORE_STRONG_ASSERT)
        return;

    int threadId = dsERASE_MANAGER_LINE_ERASER_int__GetThreadId();
    // STRONG_ASSERT(GetThreadId() >= 0 && GetThreadId() < 32)
    if ((unsigned)threadId < 0x20u ||
        (STRONG_ASSERT_DUMMY().Crash(
             "GetThreadId() >= 0 && GetThreadId() < 32",
             "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_lf_map.h",
             584,
             dsStrongAssertMessage),
         !IGNORE_STRONG_ASSERT)) {
        // STRONG_ASSERT((threadAllocMask) & (1 << GetThreadId()))
        if (((1 << threadId) & this->threadAllocMask) == 0)
            STRONG_ASSERT_DUMMY().Crash(
                "((threadAllocMask) & (1 << GetThreadId()))",
                "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_lf_map.h",
                585,
                dsStrongAssertMessage);
    }
}
