#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"

extern "C" unsigned int osGetCurThreadId();               // boundary
extern "C" void         osOutputDebugString(const char *fmt, ...); // boundary

// HALO_CHANNEL::GetState() @ 0x836C4268
// Advance the state machine (Update()) then collapse STATE into a simple tri-state:
// 0 = idle, 1 = playing (ST_PLAYING or ST_PLAYING_TO_END), 2 = anything queued/crossfading.
int HALO_CHANNEL::GetState()
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_CHANNEL::GetState", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            return 0;
        STRONG_ASSERT_DUMMY().Crash(
            "snd::THREAD_ID == osGetCurThreadId()",
            "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2045,
            empty_string);
        return 0;
    }

    this->Update();
    STATE state = this->state;
    if (state > ST_CROSSFADE)
    {
        if (!IGNORE_STRONG_ASSERT)
            STRONG_ASSERT_DUMMY().Crash(
                "false", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                2062, empty_string);
        return 0;
    }

    if (state == ST_IDLE)
        return 0;
    if (state == ST_PLAYING || state == ST_PLAYING_TO_END)
        return 1;
    return 2;
}
