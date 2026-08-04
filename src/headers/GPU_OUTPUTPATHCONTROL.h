#pragma once

/* GPU_OUTPUTPATHCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_OUTPUTPATHCONTROL). */
typedef struct _6643AD536BA1416F27C58158F69E224D { // DB types_members _6643AD536BA1416F27C58158F69E224D
    unsigned __int32 PathSelect : 2;
} _6643AD536BA1416F27C58158F69E224D;

typedef union GPU_OUTPUTPATHCONTROL { // DB types_members GPU_OUTPUTPATHCONTROL
    _6643AD536BA1416F27C58158F69E224D __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_OUTPUTPATHCONTROL;
