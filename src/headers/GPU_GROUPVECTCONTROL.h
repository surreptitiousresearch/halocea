#pragma once
#include <stdint.h>

/* GPU_GROUPVECTCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_GROUPVECTCONTROL). */
typedef struct _CBB432D2426F8A98D4D9D5591C12590E { // DB types_members _CBB432D2426F8A98D4D9D5591C12590E
    uint32_t CompXEnable : 1;
    uint32_t CompYEnable : 1;
    uint32_t CompZEnable : 1;
    uint32_t CompWEnable : 1;
    uint32_t : 4;
    uint32_t Stride : 8;
    uint32_t Shift : 8;
} _CBB432D2426F8A98D4D9D5591C12590E;

typedef union GPU_GROUPVECTCONTROL { // DB types_members GPU_GROUPVECTCONTROL
    _CBB432D2426F8A98D4D9D5591C12590E __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_GROUPVECTCONTROL;
