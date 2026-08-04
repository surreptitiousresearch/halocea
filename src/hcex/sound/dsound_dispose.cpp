#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// dsound_dispose @ 0x836B5B38 — platform_sound_manager_definition::dispose slot.
// Empty on the FMOD path (teardown handled elsewhere); retained for the vtable slot.
extern "C" void dsound_dispose(void)
{
}
