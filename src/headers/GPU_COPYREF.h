#pragma once
#include <stdint.h>

/* GPU_COPYREF - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_COPYREF). */
typedef struct _432EDF00C4C9EED3BD2386A7B2B7E909 { // DB types_members _432EDF00C4C9EED3BD2386A7B2B7E909
    uint32_t CopyRefRed : 8;
    uint32_t CopyRefGreen : 8;
    uint32_t CopyRefBlue : 8;
    uint32_t CopyRefAlpha : 8;
} _432EDF00C4C9EED3BD2386A7B2B7E909;

typedef union GPU_COPYREF { // DB types_members GPU_COPYREF
    _432EDF00C4C9EED3BD2386A7B2B7E909 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_COPYREF;
