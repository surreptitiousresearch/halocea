#include "../headers/hcex/HALO_CHANNEL_boundary.h"

// HALO_CHANNEL::Stop @ 0x836C3370 — stop both current and queued immediately
// (HALO_PERM_SOUND::Stop tears down each one's FMOD channel/banks and resets it to invalid).
void HALO_CHANNEL::Stop()
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_CHANNEL::Stop", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY::Crash(nullptr, "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2072, empty_string);
        return;
    }

    this->current.Stop();
    this->queued.Stop();
}
