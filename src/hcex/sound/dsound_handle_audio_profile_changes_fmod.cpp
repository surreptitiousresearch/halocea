#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// dsound_handle_audio_profile_changes_fmod @ 0x836B5B60 —
// platform handle_audio_profile_changes slot; no-op on FMOD path (hardware reverb/EAX
// not reconfigured at runtime).
extern "C" void dsound_handle_audio_profile_changes_fmod(unsigned char enable_hardware,
                                                         unsigned char enable_eax,
                                                         int quality_setting)
{
}
