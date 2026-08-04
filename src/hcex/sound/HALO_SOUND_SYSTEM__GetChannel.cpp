#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"

// HALO_SOUND_SYSTEM::GetChannel(int, bool) @ 0x836BF00C
// Finds the live virtual channel with slot number `virtualChannelIndex`, or (if `createNew`)
// constructs and appends a fresh one. Returns null if not found and createNew is false.
HALO_CHANNEL *HALO_SOUND_SYSTEM::GetChannel(int virtualChannelIndex, bool createNew)
{
    for (int i = 0; i < channels.nElem; ++i) {
        if (channels[i].index == virtualChannelIndex)
            return &channels[i];
    }

    if (!createNew)
        return nullptr;

    // DEVIATION: the decompiler dropped the argument to PushBack entirely (left it as an
    // unassigned local `v8`) -- disasm confirms the real call is
    // `PushBack(&this->channels, &newChannel)` where `newChannel` is this freshly
    // placement-constructed/destructed local, matching the surrounding
    // HALO_CHANNEL::HALO_CHANNEL(&newChannel, virtualChannelIndex) / ~HALO_CHANNEL(&newChannel)
    // pair (0x836BF058-0x836BF070).
    HALO_CHANNEL newChannel(virtualChannelIndex);
    HALO_CHANNEL &appended = channels.PushBack(newChannel);
    return &appended;
}
