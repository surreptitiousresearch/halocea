#pragma once

/* GPU_HICONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_HICONTROL). */
typedef struct _7CBE5399EBFEC97132850A7937B7CCD4 { // DB types_members _7CBE5399EBFEC97132850A7937B7CCD4
    unsigned __int32 HiZWriteEnable : 1;
    unsigned __int32 HiZEnable : 1;
    unsigned __int32 HiStencilWriteEnable : 1;
    unsigned __int32 HiStencilEnable : 1;
    unsigned __int32 HiZFunc : 1;
    unsigned __int32 HiStencilFunc : 1;
    unsigned __int32 : 2;
    unsigned __int32 HiStencilRef : 8;
    unsigned __int32 : 1;
    unsigned __int32 HiBaseAddr : 15;
} _7CBE5399EBFEC97132850A7937B7CCD4;

typedef union GPU_HICONTROL { // DB types_members GPU_HICONTROL
    _7CBE5399EBFEC97132850A7937B7CCD4 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_HICONTROL;
