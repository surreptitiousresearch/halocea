#pragma once

/* GPU_POINT - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_POINT). */
typedef struct _593C2A17BDECD3DDE2A8F135F2E47C31 { // DB types_members _593C2A17BDECD3DDE2A8F135F2E47C31
    __int32 X : 15;
    unsigned __int32 : 1;
    __int32 Y : 15;
} _593C2A17BDECD3DDE2A8F135F2E47C31;

typedef union GPU_POINT { // DB types_members GPU_POINT
    _593C2A17BDECD3DDE2A8F135F2E47C31 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_POINT;
