#pragma once
#include <stdint.h>

/* GPU_COLORCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_COLORCONTROL). */
typedef struct _8EED06734585DA87DE3E09A4D145E166 { // DB types_members _8EED06734585DA87DE3E09A4D145E166
    uint32_t AlphaFunc : 3;
    uint32_t AlphaTestEnable : 1;
    uint32_t AlphaToMaskEnable : 1;
    uint32_t : 19;
    uint32_t AlphaToMaskOffset0 : 2;
    uint32_t AlphaToMaskOffset1 : 2;
    uint32_t AlphaToMaskOffset2 : 2;
    uint32_t AlphaToMaskOffset3 : 2;
} _8EED06734585DA87DE3E09A4D145E166;

typedef union GPU_COLORCONTROL { // DB types_members GPU_COLORCONTROL
    _8EED06734585DA87DE3E09A4D145E166 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_COLORCONTROL;
