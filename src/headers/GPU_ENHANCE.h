#pragma once
#include <stdint.h>

/* GPU_ENHANCE - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_ENHANCE). */
typedef struct _BE2E1A1E838C74965C6FB7961EA0C7FD { // DB types_members _BE2E1A1E838C74965C6FB7961EA0C7FD
    uint32_t Misc : 16;
} _BE2E1A1E838C74965C6FB7961EA0C7FD;

typedef union GPU_ENHANCE { // DB types_members GPU_ENHANCE
    _BE2E1A1E838C74965C6FB7961EA0C7FD __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_ENHANCE;
