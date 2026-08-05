#pragma once
#include <stdint.h>

/* GPU_EDRAMMODECONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_EDRAMMODECONTROL). */
typedef struct _7B60C00327D64B8946657EB99A7AE65B { // DB types_members _7B60C00327D64B8946657EB99A7AE65B
    uint32_t EdramMode : 3;
    uint32_t ColorDepthMacro : 1;
} _7B60C00327D64B8946657EB99A7AE65B;

typedef union GPU_EDRAMMODECONTROL { // DB types_members GPU_EDRAMMODECONTROL
    _7B60C00327D64B8946657EB99A7AE65B __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_EDRAMMODECONTROL;
