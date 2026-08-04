#pragma once

/* GPU_BLENDCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_BLENDCONTROL). */
typedef struct _57A6696024BA4F1E3424679381F8140A { // DB types_members _57A6696024BA4F1E3424679381F8140A
    unsigned __int32 ColorSrcBlend : 5;
    unsigned __int32 ColorBlendOp : 3;
    unsigned __int32 ColorDestBlend : 5;
    unsigned __int32 : 3;
    unsigned __int32 AlphaSrcBlend : 5;
    unsigned __int32 AlphaBlendOp : 3;
    unsigned __int32 AlphaDestBlend : 5;
} _57A6696024BA4F1E3424679381F8140A;

typedef union GPU_BLENDCONTROL { // DB types_members GPU_BLENDCONTROL
    _57A6696024BA4F1E3424679381F8140A __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_BLENDCONTROL;
