#pragma once

/* GPU_PROGRAMPACKET - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_PROGRAMPACKET). */
#include "GPU_CONTEXTMISC.h"
#include "GPU_INTERPOLATORCONTROL.h"
#include "GPU_PROGRAMCONTROL.h"
#include "GPU_WRAPPING0.h"
#include "GPU_WRAPPING1.h"

typedef struct GPU_PROGRAMPACKET { // DB types_members GPU_PROGRAMPACKET
    GPU_PROGRAMCONTROL ProgramControl; /* 0x00 */
    GPU_CONTEXTMISC ContextMisc; /* 0x04 */
    GPU_INTERPOLATORCONTROL InterpolatorControl; /* 0x08 */
    GPU_WRAPPING0 Wrapping0; /* 0x0C */
    GPU_WRAPPING1 Wrapping1; /* 0x10 */
} GPU_PROGRAMPACKET;
