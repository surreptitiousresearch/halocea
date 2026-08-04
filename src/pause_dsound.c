#include <stdint.h>
#include "headers/blam_data_globals.h"
/* DEVIATION: HALO_SOUND_SYSTEM::Pause is a C++ virtual method; represented as C extern. */
extern void HALO_SOUND_SYSTEM_Pause(void *self, int paused);

void pause_dsound(uint8_t paused)
{
    HALO_SOUND_SYSTEM_Pause(haloSoundSystem, paused != 0);
}
