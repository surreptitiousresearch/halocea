#include "../headers/hcex/HALO_SOUND_SYSTEM.h"

// 0x836BEFF0 — look up the virtual channel with the given slot index; when `createNew` is set and
// no channel with that index exists yet, append a fresh HALO_CHANNEL and return it. Returns null
// when not found and not creating.
HALO_CHANNEL *HALO_SOUND_SYSTEM::GetChannel(int virtualChannelIndex, bool createNew)
{
    for (int i = 0; i < this->channels.nElem; ++i)
    {
        if (this->channels[i].index == virtualChannelIndex)
            return &this->channels[i];
    }

    if (createNew)
    {
        HALO_CHANNEL fresh(virtualChannelIndex);
        return &this->channels.PushBack(fresh);
    }
    return nullptr;
}
