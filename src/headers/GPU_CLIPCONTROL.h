#pragma once
#include <stdint.h>

/* GPU_CLIPCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_CLIPCONTROL). */
typedef struct _79387540FA928B751E564394007528B3 { // DB types_members _79387540FA928B751E564394007528B3
    uint32_t ClipPlaneEnable0 : 1;
    uint32_t ClipPlaneEnable1 : 1;
    uint32_t ClipPlaneEnable2 : 1;
    uint32_t ClipPlaneEnable3 : 1;
    uint32_t ClipPlaneEnable4 : 1;
    uint32_t ClipPlaneEnable5 : 1;
    uint32_t : 8;
    uint32_t ClipPlaneMode : 2;
    uint32_t ClipDisable : 1;
    uint32_t ClipPlaneCullOnlyEnable : 1;
    uint32_t BoundaryEdgeFlagEnable : 1;
    uint32_t DxClipSpaceDef : 1;
    uint32_t DisableClipErrDetect : 1;
    uint32_t VtxKillOr : 1;
    uint32_t XyNanRetain : 1;
    uint32_t ZNanRetain : 1;
    uint32_t WNanRetain : 1;
} _79387540FA928B751E564394007528B3;

typedef union GPU_CLIPCONTROL { // DB types_members GPU_CLIPCONTROL
    _79387540FA928B751E564394007528B3 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_CLIPCONTROL;
