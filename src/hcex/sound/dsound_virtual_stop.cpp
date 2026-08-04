#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// dsound_virtual_stop @ 0x836C3810 — stop a virtual channel.
extern "C" void dsound_virtual_stop(short virtual_channel_index)
{
    haloSoundSystem->dbgLock.Lock(0, 0);
    haloSoundSystem->Stop(virtual_channel_index);
    haloSoundSystem->dbgLock.Unlock(0, 0);
}
