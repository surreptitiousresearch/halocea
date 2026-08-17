/* ?AddRef@HALO_CROSSFADE_DSP@@QAAXXZ @0x836B4F18 */
// ?AddRef@HALO_CROSSFADE_DSP@@QAAXXZ -- 0x836B4F20
#include "../../headers/hcex/HALO_CROSSFADE_DSP.h"

void HALO_CROSSFADE_DSP::AddRef()
{
    ++refCount;
}
