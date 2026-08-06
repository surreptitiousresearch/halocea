#include "../../headers/d3d_driver.h"
#include "../../headers/_D3DVIEWPORT9.h"
#include "../../headers/ws/vid/vidLOCK.h"

// boundary -- the video-driver critical-section singleton.
extern vidLOCK vidLock;
// boundary -- Xbox 360 D3D SDK: set the device viewport.
extern "C" void D3DDevice_SetViewport(D3DDevice *pDevice, _D3DVIEWPORT9 *pViewport);

// 0x8268E518 -- ?SetViewport@d3dDRIVER@@... (virtual)  D3d_drv_8.cpp:2298
// Set the D3D viewport. Guarded by the device-ready state bit 0x04000000 (bit 26) and serialized
// on the video lock. A non-positive width or height requests the full-mode viewport (the render
// mode's sx/sy with a 0..1 depth range); otherwise the supplied rectangle and depth range are used.
void d3dDRIVER::SetViewport(int x, int y, int width, int height, float minZ, float maxZ)
{
    if (((this->base.state.state >> 26) & 1) == 0)
        return;

    vidLock.Lock("D:\\Projects\\code\\common\\src.sys\\drv\\video\\d3d_8\\D3d_drv_8.cpp", 2298);

    _D3DVIEWPORT9 viewport;
    if (width <= 0 || height <= 0)
    {
        vidMODE *pMode = this->base.pMode;
        viewport.X = 0;
        viewport.Y = 0;
        viewport.Width = pMode->sx;
        viewport.Height = pMode->sy;
        viewport.MaxZ = 1.0f;
        viewport.MinZ = 0.0f;
    }
    else
    {
        viewport.X = (unsigned int)x;
        viewport.Y = (unsigned int)y;
        viewport.Width = (unsigned int)width;
        viewport.Height = (unsigned int)height;
        viewport.MinZ = minZ;
        viewport.MaxZ = maxZ;
    }

    D3DDevice_SetViewport(this->pDevice, &viewport);
    vidLock.Unlock("D:\\Projects\\code\\common\\src.sys\\drv\\video\\d3d_8\\D3d_drv_8.cpp", 2298);
}
