#pragma once

/* GPU_COPYFUNC - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_COPYFUNC). */
typedef struct _C860C0CDD73E33CBA5D722430418BF2D { // DB types_members _C860C0CDD73E33CBA5D722430418BF2D
    unsigned __int32 CopyFuncRed : 3;
    unsigned __int32 : 1;
    unsigned __int32 CopyFuncGreen : 3;
    unsigned __int32 : 1;
    unsigned __int32 CopyFuncBlue : 3;
    unsigned __int32 : 1;
    unsigned __int32 CopyFuncAlpha : 3;
} _C860C0CDD73E33CBA5D722430418BF2D;

typedef union GPU_COPYFUNC { // DB types_members GPU_COPYFUNC
    _C860C0CDD73E33CBA5D722430418BF2D __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_COPYFUNC;
