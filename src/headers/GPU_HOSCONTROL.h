#pragma once
#include <stdint.h>

/* GPU_HOSCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_HOSCONTROL). */
typedef struct _9C0FA776DCE0CEF28A3080529A0833BF { // DB types_members _9C0FA776DCE0CEF28A3080529A0833BF
    uint32_t TessMode : 2;
} _9C0FA776DCE0CEF28A3080529A0833BF;

typedef union GPU_HOSCONTROL { // DB types_members GPU_HOSCONTROL
    _9C0FA776DCE0CEF28A3080529A0833BF __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_HOSCONTROL;
