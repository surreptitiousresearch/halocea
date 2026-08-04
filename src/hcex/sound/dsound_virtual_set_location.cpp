#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/sound_location.h"

// dsound_virtual_set_location @ 0x836BF0A0 — update a virtual channel's spatial location.
// The 'underwater' bool is passed straight through to HALO_CHANNEL::SetLocation.
extern "C" void dsound_virtual_set_location(short virtual_channel_index, unsigned char spatialize,
                                            const sound_location *location, float occlusion,
                                            float obstruction, unsigned char underwater,
                                            short class_index)
{
    HALO_CHANNEL *channel = haloSoundSystem->GetChannel(virtual_channel_index, true);
    channel->SetLocation(spatialize != 0, location, occlusion, obstruction, underwater != 0);
}
