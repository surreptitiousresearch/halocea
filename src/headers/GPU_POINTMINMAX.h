#pragma once

/* GPU_POINTMINMAX - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_POINTMINMAX). */
typedef struct _21F591123A23F6375C440C9F96CAEB69 { // DB types_members _21F591123A23F6375C440C9F96CAEB69
    unsigned __int32 MinSize : 16;
    unsigned __int32 MaxSize : 16;
} _21F591123A23F6375C440C9F96CAEB69;

typedef union GPU_POINTMINMAX { // DB types_members GPU_POINTMINMAX
    _21F591123A23F6375C440C9F96CAEB69 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_POINTMINMAX;
