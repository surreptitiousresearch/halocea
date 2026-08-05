#pragma once
#include <stdint.h>

/* GPU_PROGRAMCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_PROGRAMCONTROL). */
typedef struct _FAA112AB3A52849F69694CA12570EA7D { // DB types_members _FAA112AB3A52849F69694CA12570EA7D
    uint32_t VsMaxReg : 6;
    uint32_t : 2;
    uint32_t PsMaxReg : 6;
    uint32_t : 2;
    uint32_t VsResource : 1;
    uint32_t PsResource : 1;
    uint32_t ParamGen : 1;
    uint32_t GenIndexPix : 1;
    uint32_t VsExportCount : 4;
    uint32_t VsExportMode : 3;
    uint32_t PsExportZ : 1;
    uint32_t PsExportColorCount : 3;
    uint32_t GenIndexVtx : 1;
} _FAA112AB3A52849F69694CA12570EA7D;

typedef union GPU_PROGRAMCONTROL { // DB types_members GPU_PROGRAMCONTROL
    _FAA112AB3A52849F69694CA12570EA7D __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_PROGRAMCONTROL;
