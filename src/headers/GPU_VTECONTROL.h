#pragma once
#include <stdint.h>

/* GPU_VTECONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_VTECONTROL). */
typedef struct _BC1A2DDCC99D0A151A7D6FD19DE66001 { // DB types_members _BC1A2DDCC99D0A151A7D6FD19DE66001
    uint32_t VportXScaleEnable : 1;
    uint32_t VportXOffsetEnable : 1;
    uint32_t VportYScaleEnable : 1;
    uint32_t VportYOffsetEnable : 1;
    uint32_t VportZScaleEnable : 1;
    uint32_t VportZOffsetEnable : 1;
    uint32_t : 2;
    uint32_t VtxXyFmt : 1;
    uint32_t VtxZFmt : 1;
    uint32_t VtxW0Fmt : 1;
    uint32_t PerfCounterRef : 1;
} _BC1A2DDCC99D0A151A7D6FD19DE66001;

typedef union GPU_VTECONTROL { // DB types_members GPU_VTECONTROL
    _BC1A2DDCC99D0A151A7D6FD19DE66001 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_VTECONTROL;
