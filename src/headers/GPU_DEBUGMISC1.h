#pragma once

/* GPU_DEBUGMISC1 - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_DEBUGMISC1). */
typedef struct _9A74B919D61F2EE8175815F7A8FA5D76 { // DB types_members _9A74B919D61F2EE8175815F7A8FA5D76
    unsigned __int32 DbOnPix : 1;
    unsigned __int32 DbOnVtx : 1;
    unsigned __int32 : 6;
    unsigned __int32 DbInstCount : 8;
    unsigned __int32 DbBreakAddr : 11;
} _9A74B919D61F2EE8175815F7A8FA5D76;

typedef union GPU_DEBUGMISC1 { // DB types_members GPU_DEBUGMISC1
    _9A74B919D61F2EE8175815F7A8FA5D76 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_DEBUGMISC1;
