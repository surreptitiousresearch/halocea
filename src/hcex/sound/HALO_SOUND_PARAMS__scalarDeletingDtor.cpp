// ??_GHALO_SOUND_PARAMS@@QAAPAXI@Z -- 0x836B7028
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"

extern "C" void dlFree(void *ptr);

HALO_SOUND_PARAMS *HALO_SOUND_PARAMS::scalarDeletingDtor(unsigned char deleteFlags)
{
    dlFree(replace.pData);

    HALO_SOUND_PARAMS *result = this;
    if ((deleteFlags & 1) != 0)
    {
        operator delete(this);
        return this;
    }
    return result;
}
