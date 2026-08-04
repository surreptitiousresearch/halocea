#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// dsound_virtual_get_state @ 0x836C4680 — return the play-state of a virtual channel
// (0 if the channel slot is not currently allocated).
extern "C" short dsound_virtual_get_state(short virtual_channel_index)
{
    HALO_CHANNEL *channel = haloSoundSystem->GetChannel(virtual_channel_index, false);
    if (channel)
        return (short)channel->GetState();
    return 0;
}
