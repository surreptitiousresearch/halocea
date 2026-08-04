#pragma once

/* GPU_VTECONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_VTECONTROL). */
typedef struct _BC1A2DDCC99D0A151A7D6FD19DE66001 { // DB types_members _BC1A2DDCC99D0A151A7D6FD19DE66001
    unsigned __int32 VportXScaleEnable : 1;
    unsigned __int32 VportXOffsetEnable : 1;
    unsigned __int32 VportYScaleEnable : 1;
    unsigned __int32 VportYOffsetEnable : 1;
    unsigned __int32 VportZScaleEnable : 1;
    unsigned __int32 VportZOffsetEnable : 1;
    unsigned __int32 : 2;
    unsigned __int32 VtxXyFmt : 1;
    unsigned __int32 VtxZFmt : 1;
    unsigned __int32 VtxW0Fmt : 1;
    unsigned __int32 PerfCounterRef : 1;
} _BC1A2DDCC99D0A151A7D6FD19DE66001;

typedef union GPU_VTECONTROL { // DB types_members GPU_VTECONTROL
    _BC1A2DDCC99D0A151A7D6FD19DE66001 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_VTECONTROL;
