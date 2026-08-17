/* ?ClearSource@vidDRIVER_INTERFACE@@UAAXXZ @0x82666DC0 */
#include "../../headers/vidDRIVER_INTERFACE.h"

// 0x82666DC0 -- ?ClearSource@vidDRIVER_INTERFACE@@...
// Unbind every texture stage: for each of the 19 stages, call EnsureTexture with a null texture
// (pTex=nullptr, a3=0, a4=-1) which clears the stage's current source binding.
void vidDRIVER_INTERFACE::ClearSource()
{
    for (int stage = 0; stage < 19; ++stage)
        this->EnsureTexture(nullptr, stage, 0, 0xFFFFFFFFu);
}
