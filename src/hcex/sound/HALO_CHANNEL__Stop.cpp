#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"

extern "C" unsigned int osGetCurThreadId();               // boundary
extern "C" void         osOutputDebugString(const char *fmt, ...); // boundary

// HALO_CHANNEL::Stop() @ 0x836C33B0
// Stop both current and queued immediately (HALO_PERM_SOUND::Stop tears down each's FMOD
// channel/bank and resets it to invalid).
void HALO_CHANNEL::Stop()
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_CHANNEL::Stop", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash(
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2072,
                dsStrongAssertMessage);
        return;
    }

    this->current.Stop();
    this->queued.Stop();
}
