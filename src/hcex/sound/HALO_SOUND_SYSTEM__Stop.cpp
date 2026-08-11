// HALO_SOUND_SYSTEM__Stop @0x836C36D0
#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// HALO_SOUND_SYSTEM::Stop(int) @ 0x836C370C
// Stops the virtual channel with slot number `virtualChannelIndex`. If its current sound has
// HALO_SOUND_PLAY_TO_END set and is still ST_PLAYING, the channel is moved to `playToEnd`
// (letting FMOD finish the tail naturally) instead of being hard-stopped; either way the slot
// is dropped from `channels`.
void HALO_SOUND_SYSTEM::Stop(int virtualChannelIndex)
{
    if ((snd::System->state.val & 1) == 0 || snd::THREAD_ID == osGetCurThreadId()) {
        HALO_CHANNEL *channel = GetChannel(virtualChannelIndex, false);
        if (channel) {
            const HALO_SOUND_PARAMS *params = channel->current.params;
            bool playToEndFlag = params && ((params->state.val & HALO_SOUND_PLAY_TO_END) != 0);

            if (playToEndFlag && channel->state == ST_PLAYING)
                playToEnd.PushBack(*channel);
            else
                channel->Stop();

            int idx = (int)(channel - channels.Begin());
            channels.EraseNonSorted(idx);
        }
    } else {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_SOUND_SYSTEM::Stop", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("snd::THREAD_ID == osGetCurThreadId()",
                                         "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                         2480, empty_string);
    }
}
