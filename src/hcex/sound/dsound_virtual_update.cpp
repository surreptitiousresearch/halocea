#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// dsound_virtual_update @ 0x836B5B50 — platform channel_update slot; no-op on FMOD path.
extern "C" void dsound_virtual_update(short virtual_channel_index, unsigned char lapping,
                                      short class_index)
{
}
