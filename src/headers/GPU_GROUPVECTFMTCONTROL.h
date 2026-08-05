#pragma once
#include <stdint.h>

/* GPU_GROUPVECTFMTCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_GROUPVECTFMTCONTROL). */
typedef struct _0B5F7FB2731F71913AC6525C5F302169 { // DB types_members _0B5F7FB2731F71913AC6525C5F302169
    uint32_t XConv : 4;
    uint32_t XOffset : 4;
    uint32_t YConv : 4;
    uint32_t YOffset : 4;
    uint32_t ZConv : 4;
    uint32_t ZOffset : 4;
    uint32_t WConv : 4;
    uint32_t WOffset : 4;
} _0B5F7FB2731F71913AC6525C5F302169;

typedef union GPU_GROUPVECTFMTCONTROL { // DB types_members GPU_GROUPVECTFMTCONTROL
    _0B5F7FB2731F71913AC6525C5F302169 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_GROUPVECTFMTCONTROL;
