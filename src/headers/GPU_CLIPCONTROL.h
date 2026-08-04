#pragma once

/* GPU_CLIPCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_CLIPCONTROL). */
typedef struct _79387540FA928B751E564394007528B3 { // DB types_members _79387540FA928B751E564394007528B3
    unsigned __int32 ClipPlaneEnable0 : 1;
    unsigned __int32 ClipPlaneEnable1 : 1;
    unsigned __int32 ClipPlaneEnable2 : 1;
    unsigned __int32 ClipPlaneEnable3 : 1;
    unsigned __int32 ClipPlaneEnable4 : 1;
    unsigned __int32 ClipPlaneEnable5 : 1;
    unsigned __int32 : 8;
    unsigned __int32 ClipPlaneMode : 2;
    unsigned __int32 ClipDisable : 1;
    unsigned __int32 ClipPlaneCullOnlyEnable : 1;
    unsigned __int32 BoundaryEdgeFlagEnable : 1;
    unsigned __int32 DxClipSpaceDef : 1;
    unsigned __int32 DisableClipErrDetect : 1;
    unsigned __int32 VtxKillOr : 1;
    unsigned __int32 XyNanRetain : 1;
    unsigned __int32 ZNanRetain : 1;
    unsigned __int32 WNanRetain : 1;
} _79387540FA928B751E564394007528B3;

typedef union GPU_CLIPCONTROL { // DB types_members GPU_CLIPCONTROL
    _79387540FA928B751E564394007528B3 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_CLIPCONTROL;
