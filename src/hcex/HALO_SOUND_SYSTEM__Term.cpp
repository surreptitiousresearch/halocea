#include "../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../headers/ws/snd/snd_fmod_boundary.h"
#include "../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../headers/ws/ds/ds_assert_boundary.h"

extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

// 0x836BD790 — snd::SYSTEM_CUSTOM::Term override: tear down every per-level FSB sound list and
// empty the table. Must run on the owning sound thread; if the driver is live (state bit 0 set)
// and we're on the wrong thread, log + assert instead of touching FMOD state.
void HALO_SOUND_SYSTEM::Term()
{
    if ((snd::System->state.val & 1) == 0 || snd::THREAD_ID == osGetCurThreadId())
    {
        for (int i = 0; i < this->soundLists.nElem; ++i)
            this->soundLists[i].Term();
        this->soundLists.Clear();
    }
    else
    {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
            "HALO_SOUND_SYSTEM::Term", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY::Crash(nullptr,
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                2311, empty_string);
    }
}
