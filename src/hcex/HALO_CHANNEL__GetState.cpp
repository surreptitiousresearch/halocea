#include "../headers/hcex/HALO_CHANNEL_boundary.h"

// HALO_CHANNEL::GetState @ 0x836C4228 — current/queued playback state as a simple tri-state
// (0 = idle, 1 = playing normally or to-end, 2 = queued/crossfading). Runs Update() first so
// `state` reflects the latest FMOD poll.
//
// The leading block is the standard snd:: owning-thread guard shared by every public method:
// when the driver is live (snd::System->state bit0) the caller must be on snd::THREAD_ID.
int HALO_CHANNEL::GetState()
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_CHANNEL::GetState", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY::Crash(nullptr, "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2045, empty_string);
        return 0;
    }

    Update();

    HALO_CHANNEL::STATE state = this->state;
    if ((unsigned int)state > (unsigned int)ST_CROSSFADE)
    {
        if (!IGNORE_STRONG_ASSERT)
            STRONG_ASSERT_DUMMY::Crash(nullptr, "false",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2062, empty_string);
        return 0;
    }

    if (state == ST_IDLE)
        return 0;
    if (state == ST_PLAYING || state == ST_PLAYING_TO_END)
        return 1;
    return 2;
}
