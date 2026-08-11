// ??1HALO_SOUND_PARAMS@@QAA@XZ -- @0x836B6F60
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"

extern "C" void dlFree(void *ptr);

HALO_SOUND_PARAMS::~HALO_SOUND_PARAMS()
{
    dlFree(replace.pData);
}
