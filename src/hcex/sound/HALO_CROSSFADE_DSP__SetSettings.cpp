// ?SetSettings@HALO_CROSSFADE_DSP@@QAAX_KH_N@Z -- 0x836B5480
#include <stdint.h>
#include "../../headers/hcex/HALO_CROSSFADE_DSP.h"

void HALO_CROSSFADE_DSP::SetSettings(uint64_t start, int fadeTime, bool isFadeIn_)
{
    startTime = start;
    isFadeIn = isFadeIn_;
    endTime = start + fadeTime;
}
