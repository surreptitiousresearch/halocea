#include "headers/d3d_driver.h"
#include <string.h> // memset

// @0x8268F140 -- d3dDRIVER constructor. Chains the vidDRIVER base ctor (56 = video-driver
// category tag), installs the d3dDRIVER vtable, clears the D3D caps / present-parameter
// blocks and every device/surface/query pointer, and enables PWL gamma by default.
d3dDRIVER::d3dDRIVER()
{
    static const apCL k_cl = { "D:\\Projects\\code\\common\\src.sys\\drv\\video\\d3d_8\\D3d_drv_8.cpp", 686 };
    static const apCL k_clQuery = { "D:\\Projects\\code\\common\\src.sys\\drv\\video\\d3d_8\\D3d_drv_8.cpp", 688 };

    vidDRIVER::vidDRIVER(56);
    this->base.__vftable = (vidDRIVER_vtbl *)&d3dDRIVER_vftable;

    this->RTPool.pData = nullptr;
    this->RTPool.nElem = 0;
    this->RTPool.allocated = 0;
    this->RTPool.__cl = k_cl;

    this->pAllocator = nullptr;

    this->queryList.pData = nullptr;
    this->queryList.nElem = 0;
    this->queryList.allocated = 0;
    this->queryList.__cl = k_clQuery; // decompiler folded {file,line=688} into a 0x130000002B0 constant

    memset(&this->caps, 0, sizeof(this->caps));
    memset(&this->presentParam, 0, sizeof(this->presentParam));

    this->base.vidMemSize = 0;
    this->enablePWLGamma = 1;
    this->pD3D = nullptr;
    this->pDevice = nullptr;
    this->pBackBuffer = nullptr;
    this->GpuTimeQueryInit();
    this->pDepthBufTex = nullptr;
    this->pDepthBuffer = nullptr;
    this->base.pQueuedScreenshot = nullptr;
}
