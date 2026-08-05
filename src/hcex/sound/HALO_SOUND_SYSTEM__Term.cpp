#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// HALO_SOUND_SYSTEM::Term() @ 0x836BD7C8
// Terminates every loaded FSB sound-list and clears the table. Must run on the owning sound
// thread (same wrong-thread guard pattern used by every other public entry point).
void HALO_SOUND_SYSTEM::Term()
{
    if ((snd::System->state.val & 1) == 0 || snd::THREAD_ID == osGetCurThreadId()) {
        for (int i = 0; i < soundLists.nElem; ++i)
            soundLists[i].Term();
        soundLists.Clear();
    } else {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_SOUND_SYSTEM::Term", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash("snd::THREAD_ID == osGetCurThreadId()",
                                         "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                         2311, empty_string);
    }
}
