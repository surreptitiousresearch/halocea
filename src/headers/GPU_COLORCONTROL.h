#pragma once

/* GPU_COLORCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_COLORCONTROL). */
typedef struct _8EED06734585DA87DE3E09A4D145E166 { // DB types_members _8EED06734585DA87DE3E09A4D145E166
    unsigned __int32 AlphaFunc : 3;
    unsigned __int32 AlphaTestEnable : 1;
    unsigned __int32 AlphaToMaskEnable : 1;
    unsigned __int32 : 19;
    unsigned __int32 AlphaToMaskOffset0 : 2;
    unsigned __int32 AlphaToMaskOffset1 : 2;
    unsigned __int32 AlphaToMaskOffset2 : 2;
    unsigned __int32 AlphaToMaskOffset3 : 2;
} _8EED06734585DA87DE3E09A4D145E166;

typedef union GPU_COLORCONTROL { // DB types_members GPU_COLORCONTROL
    _8EED06734585DA87DE3E09A4D145E166 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_COLORCONTROL;
