#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// dsound_virtual_queue @ 0x836C4F10 — queue a permutation to a virtual channel.
// class_index/is_local_player/lapping are unused on the FMOD path.
extern "C" void dsound_virtual_queue(short virtual_channel_index, const sound_permutation *sound,
                                     int identifier, unsigned char is_local_player,
                                     short class_index, unsigned char lapping)
{
    haloSoundSystem->dbgLock.Lock(0, 0);
    HALO_CHANNEL *channel = haloSoundSystem->GetChannel(virtual_channel_index, true);
    channel->Queue(sound, identifier);
    haloSoundSystem->dbgLock.Unlock(0, 0);
}
