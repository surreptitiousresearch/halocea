/* ?ResetFadeIn@HALO_PERM_SOUND@@QAAXXZ @0x836C1ED8 */
// ?ResetFadeIn@HALO_PERM_SOUND@@QAAXXZ -- 0x836C1EF4
#include "../../headers/hcex/HALO_PERM_SOUND.h"
#include "../../headers/hcex/HALO_CROSSFADE_DSP.h"

void HALO_PERM_SOUND::ResetFadeIn()
{
    previousChannel = nullptr;

    if (prevFadeOut)
    {
        prevFadeOut->Detach();
        prevFadeOut->Release();
        prevFadeOut = nullptr;
    }

    if (fadeIn)
        fadeIn->dsp->setBypass(true);
}
