#pragma once
#include <stdint.h>

/* GPU_COPYMASK - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_COPYMASK). */
typedef struct _E8E0F97045669E6EDC91D0E0D98B4BB3 { // DB types_members _E8E0F97045669E6EDC91D0E0D98B4BB3
    uint32_t CopyMaskRed : 8;
    uint32_t CopyMaskGreen : 8;
    uint32_t CopyMaskBlue : 8;
    uint32_t CopyMaskAlpha : 8;
} _E8E0F97045669E6EDC91D0E0D98B4BB3;

typedef union GPU_COPYMASK { // DB types_members GPU_COPYMASK
    _E8E0F97045669E6EDC91D0E0D98B4BB3 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_COPYMASK;
