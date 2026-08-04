// ?SetSplitScreen@ENGINE_CONTROL@@QAAX_N@Z @ 0x82768560
// Toggle split-screen rendering: reconfigure camera 0 (and create/destroy camera 1) for the new
// mode, mark visibility as needing an update, then signal the "OnSplitScreen" event.
#include "../../headers/ws/ENGINE_CONTROL.h"
#include "../../headers/ws/cam/camCAMERA.h"
#include "../../headers/vidDRIVER.h"
#include "../../headers/vidMODE.h"
#include "../../headers/ws/gs/gsRENDER_SYSTEM.h"
#include "../../headers/ws/ds/dsEVENT_MGR.h"
#include "../../headers/ws/ds/dsSTRID.h"

// --- boundary globals ---
extern vidDRIVER        *vidDriver;   // active video driver
extern gsRENDER_SYSTEM  *gsSysRender; // render system singleton
extern dsEVENT_MGR      *gEventMgr;   // global event manager

// --- boundary subsystem entry points (camera pool) ---
extern camCAMERA *gsCameraGet(int idx);
extern void       gsCameraCreate();
extern void       gsCameraDestroy(int idx);

// Function-local static: one-time OnSplitScreen event registration.
static int  evId;    // registered event id
static char _S3_16;  // guard byte for the one-time RegisterEvent block

void ENGINE_CONTROL::SetSplitScreen(bool isOn)
{
    if (this->isSplitScreen != isOn)
    {
        this->isSplitScreen = isOn;

        vidMODE *pMode = vidDriver->pMode;
        // aspect of the full render target, corrected by the pixel aspect ratio
        float aspect = (float)pMode->syRT / (float)pMode->sxRT / pMode->pixel_aspect;

        if (this->isSplitScreen)
        {
            // Top viewport (camera 0): half-height window, half the aspect.
            camCAMERA *cam0 = gsCameraGet(0);
            cam0->SetAspectRatioHW(aspect * 0.5f);
            cam0->SetViewWindow(0.0f, 0.0f, (float)pMode->sxRT, (float)pMode->syRT * 0.5f);

            // Bottom viewport (camera 1): created on demand, same geometry.
            gsCameraCreate();
            camCAMERA *cam1 = gsCameraGet(1);
            cam1->SetAspectRatioHW(aspect * 0.5f);
            cam1->SetViewWindow(0.0f, 0.0f, (float)pMode->sxRT, (float)pMode->syRT * 0.5f);
        }
        else
        {
            gsCameraDestroy(1);
            camCAMERA *cam0 = gsCameraGet(0);
            cam0->SetAspectRatioHW(aspect);
        }

        gsSysRender->isNeedUpdateVis = true;
    }

    if ((_S3_16 & 1) == 0)
    {
        _S3_16 |= 1;
        dsSTRID name("OnSplitScreen", false);
        evId = gEventMgr->RegisterEvent(name);
    }
    gEventMgr->SignalEvent(evId, nullptr);
}
