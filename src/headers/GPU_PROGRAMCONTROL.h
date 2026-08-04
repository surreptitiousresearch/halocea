#pragma once

/* GPU_PROGRAMCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_PROGRAMCONTROL). */
typedef struct _FAA112AB3A52849F69694CA12570EA7D { // DB types_members _FAA112AB3A52849F69694CA12570EA7D
    unsigned __int32 VsMaxReg : 6;
    unsigned __int32 : 2;
    unsigned __int32 PsMaxReg : 6;
    unsigned __int32 : 2;
    unsigned __int32 VsResource : 1;
    unsigned __int32 PsResource : 1;
    unsigned __int32 ParamGen : 1;
    unsigned __int32 GenIndexPix : 1;
    unsigned __int32 VsExportCount : 4;
    unsigned __int32 VsExportMode : 3;
    unsigned __int32 PsExportZ : 1;
    unsigned __int32 PsExportColorCount : 3;
    unsigned __int32 GenIndexVtx : 1;
} _FAA112AB3A52849F69694CA12570EA7D;

typedef union GPU_PROGRAMCONTROL { // DB types_members GPU_PROGRAMCONTROL
    _FAA112AB3A52849F69694CA12570EA7D __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_PROGRAMCONTROL;
