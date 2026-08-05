#pragma once
#include <stdint.h>

/* GPU_COPYDESTINFO - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_COPYDESTINFO). */
typedef struct _B8896BD11216329B217F29E1FA61C394 { // DB types_members _B8896BD11216329B217F29E1FA61C394
    uint32_t CopyDestEndian : 3;
    uint32_t CopyDestArray : 1;
    uint32_t CopyDestSlice : 3;
    uint32_t CopyDestFormat : 6;
    uint32_t CopyDestNumber : 3;
    uint32_t CopyDestExpBias : 6;
    uint32_t : 2;
    uint32_t CopyDestSwap : 1;
} _B8896BD11216329B217F29E1FA61C394;

typedef union GPU_COPYDESTINFO { // DB types_members GPU_COPYDESTINFO
    _B8896BD11216329B217F29E1FA61C394 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_COPYDESTINFO;
