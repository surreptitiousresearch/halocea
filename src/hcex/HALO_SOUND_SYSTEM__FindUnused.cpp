#include "../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../headers/ws/snd/snd_fmod_boundary.h"
#include "../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../headers/ws/ds/ds_assert_boundary.h"

extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

// 0x836BEE50 — pick a preloaded-but-unused sound belonging to `soundList` that can be recycled:
// the one that has been sitting idle the longest (largest osGetTime()-initTime age, with a 5s
// grace bonus for entries that were never actually played). Returns its preloadList index, or -1
// if none qualifies (nothing old enough / list not yet full). Must run on the owning sound thread.
int HALO_SOUND_SYSTEM::FindUnused(HALO_SOUND_LIST *soundList, bool unload)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
            "HALO_SOUND_SYSTEM::FindUnused", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY::Crash(nullptr,
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                1518, empty_string);
        return -1;
    }

    unsigned int now = osGetTime();
    int bestAge = 0;
    int bestIndex = -1;

    int count = this->preloadList.nElem;
    if (count <= 0)
        return -1;

    for (int i = 0; i < count; ++i)
    {
        if (this->preloadList[i].soundList == soundList && !this->preloadList[i].isUsed)
        {
            int age = now - this->preloadList[i].initTime;
            if (!this->preloadList[i].wasUsed)
                age -= 5000; // never-played entries are 5s "younger" (preferred less)
            if (bestIndex == -1 || bestAge < age)
            {
                bestAge = age;
                bestIndex = i;
            }
        }
        count = this->preloadList.nElem;
    }

    if (bestIndex == -1 || (bestAge < 500 && count < 32))
        return -1;

    // DEVIATION: the binary performs a bounds-checked access of preloadList[bestIndex] here when
    // `unload` is set, but discards the result (the unload work is inlined away to nothing); the
    // access is reproduced faithfully as a side-effect-free statement.
    if (unload)
        (void)this->preloadList[bestIndex];

    return bestIndex;
}
