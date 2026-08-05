#pragma once
#include <stdint.h>

/* GPU_BLENDCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_BLENDCONTROL). */
typedef struct _57A6696024BA4F1E3424679381F8140A { // DB types_members _57A6696024BA4F1E3424679381F8140A
    uint32_t ColorSrcBlend : 5;
    uint32_t ColorBlendOp : 3;
    uint32_t ColorDestBlend : 5;
    uint32_t : 3;
    uint32_t AlphaSrcBlend : 5;
    uint32_t AlphaBlendOp : 3;
    uint32_t AlphaDestBlend : 5;
} _57A6696024BA4F1E3424679381F8140A;

typedef union GPU_BLENDCONTROL { // DB types_members GPU_BLENDCONTROL
    _57A6696024BA4F1E3424679381F8140A __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_BLENDCONTROL;
