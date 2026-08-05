#pragma once
#include <stdint.h>

/* GPU_VTXCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_VTXCONTROL). */
typedef struct _DBE43B421104E02770B44F2F33EBDBD8 { // DB types_members _DBE43B421104E02770B44F2F33EBDBD8
    uint32_t PixCenter : 1;
    uint32_t RoundMode : 2;
    uint32_t QuantMode : 3;
} _DBE43B421104E02770B44F2F33EBDBD8;

typedef union GPU_VTXCONTROL { // DB types_members GPU_VTXCONTROL
    _DBE43B421104E02770B44F2F33EBDBD8 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_VTXCONTROL;
