/* play_custom_impulse @0x836C46D0 */
#include "headers/blam_data_globals.h"
/* DEVIATION: HALO_SOUND_SYSTEM::PlayCustomSound is a C++ virtual method on haloSoundSystem.
 * Represented here as a C-callable extern matching the mangled calling convention. */
extern void HALO_SOUND_SYSTEM_PlayCustomSound(void *self, const char *sound_def_name, const char *name);

void play_custom_impulse(const char *sound_def_name, const char *name)
{
    HALO_SOUND_SYSTEM_PlayCustomSound(haloSoundSystem, sound_def_name, name);
}
