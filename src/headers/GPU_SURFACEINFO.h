#pragma once
#include <stdint.h>

/* GPU_SURFACEINFO - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_SURFACEINFO). */
typedef struct _1465ECF8D76A739C5C4E5810C498A572 { // DB types_members _1465ECF8D76A739C5C4E5810C498A572
    uint32_t SurfacePitch : 14;
    uint32_t : 2;
    uint32_t MsaaSamples : 2;
    uint32_t HiZPitch : 14;
} _1465ECF8D76A739C5C4E5810C498A572;

typedef union GPU_SURFACEINFO { // DB types_members GPU_SURFACEINFO
    _1465ECF8D76A739C5C4E5810C498A572 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_SURFACEINFO;
