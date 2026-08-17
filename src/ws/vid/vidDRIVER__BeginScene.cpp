/* ?BeginScene@vidDRIVER@@UAAXXZ @0x82669190 */
#include "../../headers/vidDRIVER.h"

// 0x82669190 -- ?BeginScene@vidDRIVER@@UAAXXZ
// Open a render frame. Applies any pending gamma configuration, then publishes the just-finished
// frame's polygon/split-batch counts (nPolyFrame/nSplitFrame) from the running counters and zeroes
// the running counters for the new frame. Finally sets the "in scene" state bit 0x10000000.
void vidDRIVER::BeginScene()
{
    this->ConfigureGamma();

    int polyFrame = this->nPolyFrameCur;
    int splitFrame = this->nSplitFrameCur;
    this->nSplitFrameCur = 0;
    this->nPolyFrameCur = 0;
    this->nPolyFrame = polyFrame;
    this->nSplitFrame = splitFrame;

    this->state.state |= 0x10000000u;
}
