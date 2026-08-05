#pragma once
#include <stdint.h>

/* GPU_WRAPPING1 - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_WRAPPING1). */
typedef struct _6980EB084C9E7CE5E4C7E4558ECFD5EB { // DB types_members _6980EB084C9E7CE5E4C7E4558ECFD5EB
    uint32_t Wrap0 : 4;
    uint32_t Wrap1 : 4;
    uint32_t Wrap2 : 4;
    uint32_t Wrap3 : 4;
    uint32_t Wrap4 : 4;
    uint32_t Wrap5 : 4;
    uint32_t Wrap6 : 4;
    uint32_t Wrap7 : 4;
} _6980EB084C9E7CE5E4C7E4558ECFD5EB;

typedef union GPU_WRAPPING1 { // DB types_members GPU_WRAPPING1
    _6980EB084C9E7CE5E4C7E4558ECFD5EB __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_WRAPPING1;
