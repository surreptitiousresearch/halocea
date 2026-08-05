#pragma once
#include <stdint.h>

/* GPU_HICONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_HICONTROL). */
typedef struct _7CBE5399EBFEC97132850A7937B7CCD4 { // DB types_members _7CBE5399EBFEC97132850A7937B7CCD4
    uint32_t HiZWriteEnable : 1;
    uint32_t HiZEnable : 1;
    uint32_t HiStencilWriteEnable : 1;
    uint32_t HiStencilEnable : 1;
    uint32_t HiZFunc : 1;
    uint32_t HiStencilFunc : 1;
    uint32_t : 2;
    uint32_t HiStencilRef : 8;
    uint32_t : 1;
    uint32_t HiBaseAddr : 15;
} _7CBE5399EBFEC97132850A7937B7CCD4;

typedef union GPU_HICONTROL { // DB types_members GPU_HICONTROL
    _7CBE5399EBFEC97132850A7937B7CCD4 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_HICONTROL;
