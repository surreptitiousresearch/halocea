#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// HALO_CHANNEL::~HALO_CHANNEL() @ 0x836B5AA0
// Debug-only: strong-assert that neither current nor queued still owns a crossfade DSP
// (prevFadeOut/fadeIn) at destruction time -- those must have been released/detached by
// HALO_PERM_SOUND::Stop before a channel is torn down.
HALO_CHANNEL::~HALO_CHANNEL()
{
    if (!IGNORE_STRONG_ASSERT)
    {
        if (this->queued.prevFadeOut || this->queued.fadeIn)
            STRONG_ASSERT_DUMMY().Crash(
                "prevFadeOut == 0 && fadeIn == 0",
                "d:\\projects\\code\\hcex\\sources\\sound\\fmod\\sound_dsound_fmod.h", 260,
                empty_string);

        if (this->current.prevFadeOut || this->current.fadeIn)
            STRONG_ASSERT_DUMMY().Crash(
                "prevFadeOut == 0 && fadeIn == 0",
                "d:\\projects\\code\\hcex\\sources\\sound\\fmod\\sound_dsound_fmod.h", 260,
                empty_string);
    }
}
