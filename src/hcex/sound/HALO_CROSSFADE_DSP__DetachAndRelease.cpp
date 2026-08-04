// ?DetachAndRelease@HALO_CROSSFADE_DSP@@SAXAAPAU1@@Z -- 0x836C1C5C
#include "../../headers/hcex/HALO_CROSSFADE_DSP.h"

void HALO_CROSSFADE_DSP::DetachAndRelease(HALO_CROSSFADE_DSP *&ptr)
{
    HALO_CROSSFADE_DSP *dsp = ptr;
    if (dsp)
    {
        dsp->Detach();
        dsp->Release();
        ptr = nullptr;
    }
}
