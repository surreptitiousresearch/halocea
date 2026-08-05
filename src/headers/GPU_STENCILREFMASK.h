#pragma once
#include <stdint.h>

/* GPU_STENCILREFMASK - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_STENCILREFMASK). */
typedef struct _2A54E4DB9BFF6BD09CD1311E86BF3DD5 { // DB types_members _2A54E4DB9BFF6BD09CD1311E86BF3DD5
    uint32_t Ref : 8;
    uint32_t Mask : 8;
    uint32_t WriteMask : 8;
} _2A54E4DB9BFF6BD09CD1311E86BF3DD5;

typedef union GPU_STENCILREFMASK { // DB types_members GPU_STENCILREFMASK
    _2A54E4DB9BFF6BD09CD1311E86BF3DD5 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_STENCILREFMASK;
