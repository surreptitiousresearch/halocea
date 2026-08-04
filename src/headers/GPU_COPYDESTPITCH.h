#pragma once

/* GPU_COPYDESTPITCH - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_COPYDESTPITCH). */
typedef struct _A50DF762E74DA0F90A859E696D40BB21 { // DB types_members _A50DF762E74DA0F90A859E696D40BB21
    unsigned __int32 CopyDestPitch : 14;
    unsigned __int32 : 2;
    unsigned __int32 CopyDestHeight : 14;
} _A50DF762E74DA0F90A859E696D40BB21;

typedef union GPU_COPYDESTPITCH { // DB types_members GPU_COPYDESTPITCH
    _A50DF762E74DA0F90A859E696D40BB21 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_COPYDESTPITCH;
