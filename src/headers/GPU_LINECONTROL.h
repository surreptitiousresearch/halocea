#pragma once
#include <stdint.h>

/* GPU_LINECONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_LINECONTROL). */
typedef struct _DE593D6BCAF6191E3F2602A4EE440B31 { // DB types_members _DE593D6BCAF6191E3F2602A4EE440B31
    uint32_t Width : 16;
} _DE593D6BCAF6191E3F2602A4EE440B31;

typedef union GPU_LINECONTROL { // DB types_members GPU_LINECONTROL
    _DE593D6BCAF6191E3F2602A4EE440B31 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_LINECONTROL;
