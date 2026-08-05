#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// HALO_SOUND_SYSTEM::FindUnused(HALO_SOUND_LIST*, bool) @ 0x836BEE90
// Scans the preload list for an unused (not currently checked-out) entry belonging to
// `soundList`, picking the one that has sat idle longest (a not-yet-ever-used entry is biased
// 5 seconds "older" so fresh preloads are recycled before ones that have actually played).
// Returns -1 if nothing qualifies, or if the winning candidate isn't stale enough yet (< 500ms
// idle) while the pool is still small (< 32 entries). `unload` is read by disasm but the actual
// unload/removal call the decompiled body reaches for is a dead/no-op indexing expression
// (`operator[](preloadList, best)` with its result discarded) -- DEVIATION: reproduced
// verbatim rather than invented as a real removal, since disasm shows no store or dtor call
// following it.
int HALO_SOUND_SYSTEM::FindUnused(HALO_SOUND_LIST *soundList, bool unload)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId()) {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_SOUND_SYSTEM::FindUnused", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash("snd::THREAD_ID == osGetCurThreadId()",
                                         "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                         1518, empty_string);
        return -1;
    }

    unsigned int now = osGetTime();
    int bestIdleTime = 0;
    int bestIndex = -1;

    if (preloadList.nElem <= 0)
        return -1;

    int i = 0;
    int nElem;
    do {
        HALO_PERM_SOUND &entry = preloadList[i];
        if (entry.soundList == soundList && !entry.isUsed) {
            int idleTime = (int)now - entry.initTime;
            if (!entry.wasUsed)
                idleTime -= 5000;
            if (bestIndex == -1 || bestIdleTime < idleTime) {
                bestIdleTime = idleTime;
                bestIndex = i;
            }
        }
        nElem = preloadList.nElem;
        ++i;
    } while (i < nElem);

    if (bestIndex == -1 || (bestIdleTime < 500 && nElem < 32))
        return -1;

    if (unload)
        preloadList[bestIndex]; // DEVIATION: result discarded verbatim, see file header comment

    return bestIndex;
}
