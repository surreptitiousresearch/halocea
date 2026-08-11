#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/ws/snd/snd_fmod_boundary.h"
#include "../../headers/ws/os/os_boundary.h"

// dsound_virtual_set_properties @ 0x836BF118 — update a virtual channel's gain/pitch/etc.
// Must run on the sound thread; a non-gain-only call from another thread strong-asserts.
extern "C" void dsound_virtual_set_properties(short virtual_channel_index,
                                              const platform_sound_channel_properties *properties,
                                              unsigned char gain_only, unsigned char is_local_player,
                                              short class_index)
{
    if (snd::THREAD_ID == osGetCurThreadId()) {
        HALO_CHANNEL *channel = haloSoundSystem->GetChannel(virtual_channel_index, true);
        channel->SetProperties(properties, gain_only != 0, class_index);
    } else if (!IGNORE_STRONG_ASSERT && !gain_only) {
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("gain_only",
                                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", /* backslashes escaped 2026-07-30 */
                                    2726, empty_string);
    }
}
