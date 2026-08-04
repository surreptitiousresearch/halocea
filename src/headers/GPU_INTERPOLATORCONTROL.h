#pragma once

/* GPU_INTERPOLATORCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_INTERPOLATORCONTROL). */
typedef struct _034F27AA47666CDC33815214F368309E { // DB types_members _034F27AA47666CDC33815214F368309E
    unsigned __int32 ParamShade : 16;
    unsigned __int32 SamplingPattern : 16;
} _034F27AA47666CDC33815214F368309E;

typedef union GPU_INTERPOLATORCONTROL { // DB types_members GPU_INTERPOLATORCONTROL
    _034F27AA47666CDC33815214F368309E __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_INTERPOLATORCONTROL;
