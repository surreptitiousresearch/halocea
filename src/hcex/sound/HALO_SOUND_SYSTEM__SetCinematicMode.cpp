#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"

// HALO_SOUND_SYSTEM::SetCinematicMode(bool) @ 0x836B5188
// Flips the flag FindSoundByName later checks to gate non-dialog sounds during cinematics.
void HALO_SOUND_SYSTEM::SetCinematicMode(bool enable)
{
    isCinematicMode = enable;
}
