#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"

// HALO_SOUND_SYSTEM::UpdateCustomSounds() @ 0x836BEA5C
// Advances every in-flight custom sound; any slot whose UpdateCustomSound() reports "finished"
// is compacted out of the vector in place (EraseNonSorted swaps the last element into the
// erased slot, so the loop index is stepped back one to revisit that swapped-in element).
void HALO_SOUND_SYSTEM::UpdateCustomSounds()
{
    for (int i = 0; i < customSounds.nElem; ++i) {
        if (!UpdateCustomSound(customSounds[i]))
            customSounds.EraseNonSorted(i--);
    }
}
