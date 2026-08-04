#pragma once

/* GPU_POINTSIZE - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_POINTSIZE). */
typedef struct _B4E993025E5411723125381D6BF22EED { // DB types_members _B4E993025E5411723125381D6BF22EED
    unsigned __int32 Height : 16;
    unsigned __int32 Width : 16;
} _B4E993025E5411723125381D6BF22EED;

typedef union GPU_POINTSIZE { // DB types_members GPU_POINTSIZE
    _B4E993025E5411723125381D6BF22EED __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_POINTSIZE;
