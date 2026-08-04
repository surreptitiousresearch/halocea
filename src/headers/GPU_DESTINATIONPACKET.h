#pragma once

/* GPU_DESTINATIONPACKET - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_DESTINATIONPACKET). */
#include "GPU_COLORINFO.h"
#include "GPU_DEPTHINFO.h"
#include "GPU_POINT.h"
#include "GPU_SURFACEINFO.h"

typedef struct GPU_DESTINATIONPACKET { // DB types_members GPU_DESTINATIONPACKET
    GPU_SURFACEINFO SurfaceInfo; /* 0x00 */
    GPU_COLORINFO Color0Info; /* 0x04 */
    GPU_DEPTHINFO DepthInfo; /* 0x08 */
    GPU_COLORINFO Color1Info; /* 0x0C */
    GPU_COLORINFO Color2Info; /* 0x10 */
    GPU_COLORINFO Color3Info; /* 0x14 */
    unsigned int CoherDestBase0; /* 0x18 */
    unsigned int CoherDestBase1; /* 0x1C */
    unsigned int CoherDestBase2; /* 0x20 */
    unsigned int CoherDestBase3; /* 0x24 */
    unsigned int CoherDestBase4; /* 0x28 */
    unsigned int CoherDestBase5; /* 0x2C */
    unsigned int CoherDestBase6; /* 0x30 */
    unsigned int CoherDestBase7; /* 0x34 */
    GPU_POINT ScreenScissorTL; /* 0x38 */
    GPU_POINT ScreenScissorBR; /* 0x3C */
} GPU_DESTINATIONPACKET;
