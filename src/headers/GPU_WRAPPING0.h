#pragma once
#include <stdint.h>

/* GPU_WRAPPING0 - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_WRAPPING0). */
typedef struct _C829270B0534D0C269A8C45BC9037B1C { // DB types_members _C829270B0534D0C269A8C45BC9037B1C
    uint32_t Wrap8 : 4;
    uint32_t Wrap9 : 4;
    uint32_t Wrap10 : 4;
    uint32_t Wrap11 : 4;
    uint32_t Wrap12 : 4;
    uint32_t Wrap13 : 4;
    uint32_t Wrap14 : 4;
    uint32_t Wrap15 : 4;
} _C829270B0534D0C269A8C45BC9037B1C;

typedef union GPU_WRAPPING0 { // DB types_members GPU_WRAPPING0
    _C829270B0534D0C269A8C45BC9037B1C __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_WRAPPING0;
