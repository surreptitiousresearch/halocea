/* ?EndScene@vidDRIVER@@UAAXXZ @0x8266E418 */
#include "../../headers/vidDRIVER.h"
#include "../../headers/vid_boundary.h" /* vidVBMng / vidVBUF_MNG__DiscardDynAll */

// 0x8266E418 -- ?EndScene@vidDRIVER@@UAAXXZ
// Close a render frame and reset transient device state for the next one: unbind the vertex and
// pixel shaders and the shader-input declaration, unbind all vertex streams, discard the frame's
// dynamic geometry, invalidate the cached device state and clear the current source bindings,
// restore the viewport to the full display mode, finalize the DIP statistics, clear the "in scene"
// state bit 0x10000000, and advance the global frame counter.
void vidDRIVER::EndScene()
{
    this->drvInterface->EnsureVertShader(nullptr);
    this->drvInterface->EnsurePixShader(nullptr, 0);
    this->drvInterface->EnsureShaderDecl(nullptr, 0, 0);
    this->ResetAllStreams();
    vidVBUF_MNG__DiscardDynAll(vidVBMng);
    this->drvInterface->Invalidate();
    this->drvInterface->ClearSource();
    this->SetViewport(0, 0, this->pMode->sx, this->pMode->sy, 0.0f, 1.0f);
    this->dipStat.FinishFrame();
    this->state.state &= ~0x10000000u;
    ++vidDRIVER::frameNumber;
}
