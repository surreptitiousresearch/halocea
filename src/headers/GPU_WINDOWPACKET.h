#pragma once

/* GPU_WINDOWPACKET - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_WINDOWPACKET). */
#include "GPU_POINT.h"

typedef struct GPU_WINDOWPACKET { // DB types_members GPU_WINDOWPACKET
    GPU_POINT WindowOffset; /* 0x00 */
    GPU_POINT WindowScissorTL; /* 0x04 */
    GPU_POINT WindowScissorBR; /* 0x08 */
} GPU_WINDOWPACKET;
