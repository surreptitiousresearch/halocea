#include "../headers/hcex/HALO_SOUND_SYSTEM.h"

// 0x836BEA48 — advance every one-off "custom" sound; unordered-remove any that reports finished
// (UpdateCustomSound returns false). The index is rewound on erase so the swapped-in tail entry
// is not skipped.
void HALO_SOUND_SYSTEM::UpdateCustomSounds()
{
    for (int i = 0; i < this->customSounds.nElem; ++i)
    {
        if (!UpdateCustomSound(this->customSounds[i]))
            this->customSounds.EraseNonSorted(i--);
    }
}
