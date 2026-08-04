#pragma once

/* GPU_DEPTHINFO - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_DEPTHINFO). */
typedef struct _EC8D9EAADE278BDE148C83773DD59CA9 { // DB types_members _EC8D9EAADE278BDE148C83773DD59CA9
    unsigned __int32 DepthBase : 12;
    unsigned __int32 : 4;
    unsigned __int32 DepthFormat : 1;
    unsigned __int32 DisableHZClamp : 1;
} _EC8D9EAADE278BDE148C83773DD59CA9;

typedef union GPU_DEPTHINFO { // DB types_members GPU_DEPTHINFO
    _EC8D9EAADE278BDE148C83773DD59CA9 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_DEPTHINFO;
