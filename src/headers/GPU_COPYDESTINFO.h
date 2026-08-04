#pragma once

/* GPU_COPYDESTINFO - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_COPYDESTINFO). */
typedef struct _B8896BD11216329B217F29E1FA61C394 { // DB types_members _B8896BD11216329B217F29E1FA61C394
    unsigned __int32 CopyDestEndian : 3;
    unsigned __int32 CopyDestArray : 1;
    unsigned __int32 CopyDestSlice : 3;
    unsigned __int32 CopyDestFormat : 6;
    unsigned __int32 CopyDestNumber : 3;
    unsigned __int32 CopyDestExpBias : 6;
    unsigned __int32 : 2;
    unsigned __int32 CopyDestSwap : 1;
} _B8896BD11216329B217F29E1FA61C394;

typedef union GPU_COPYDESTINFO { // DB types_members GPU_COPYDESTINFO
    _B8896BD11216329B217F29E1FA61C394 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_COPYDESTINFO;
