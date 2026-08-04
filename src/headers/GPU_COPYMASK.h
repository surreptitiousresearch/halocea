#pragma once

/* GPU_COPYMASK - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_COPYMASK). */
typedef struct _E8E0F97045669E6EDC91D0E0D98B4BB3 { // DB types_members _E8E0F97045669E6EDC91D0E0D98B4BB3
    unsigned __int32 CopyMaskRed : 8;
    unsigned __int32 CopyMaskGreen : 8;
    unsigned __int32 CopyMaskBlue : 8;
    unsigned __int32 CopyMaskAlpha : 8;
} _E8E0F97045669E6EDC91D0E0D98B4BB3;

typedef union GPU_COPYMASK { // DB types_members GPU_COPYMASK
    _E8E0F97045669E6EDC91D0E0D98B4BB3 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_COPYMASK;
