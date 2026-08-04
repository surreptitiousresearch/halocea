#pragma once

/* GPU_GROUPVECTCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_GROUPVECTCONTROL). */
typedef struct _CBB432D2426F8A98D4D9D5591C12590E { // DB types_members _CBB432D2426F8A98D4D9D5591C12590E
    unsigned __int32 CompXEnable : 1;
    unsigned __int32 CompYEnable : 1;
    unsigned __int32 CompZEnable : 1;
    unsigned __int32 CompWEnable : 1;
    unsigned __int32 : 4;
    unsigned __int32 Stride : 8;
    unsigned __int32 Shift : 8;
} _CBB432D2426F8A98D4D9D5591C12590E;

typedef union GPU_GROUPVECTCONTROL { // DB types_members GPU_GROUPVECTCONTROL
    _CBB432D2426F8A98D4D9D5591C12590E __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_GROUPVECTCONTROL;
