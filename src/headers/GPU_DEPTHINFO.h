#pragma once
#include <stdint.h>

/* GPU_DEPTHINFO - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_DEPTHINFO). */
typedef struct _EC8D9EAADE278BDE148C83773DD59CA9 { // DB types_members _EC8D9EAADE278BDE148C83773DD59CA9
    uint32_t DepthBase : 12;
    uint32_t : 4;
    uint32_t DepthFormat : 1;
    uint32_t DisableHZClamp : 1;
} _EC8D9EAADE278BDE148C83773DD59CA9;

typedef union GPU_DEPTHINFO { // DB types_members GPU_DEPTHINFO
    _EC8D9EAADE278BDE148C83773DD59CA9 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_DEPTHINFO;
