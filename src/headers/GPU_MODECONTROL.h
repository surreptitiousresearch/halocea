#pragma once

/* GPU_MODECONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_MODECONTROL). */
typedef struct _94AFD67874D6A703F84CC15A9C8F6192 { // DB types_members _94AFD67874D6A703F84CC15A9C8F6192
    unsigned __int32 CullMode : 3;
    unsigned __int32 PolyMode : 2;
    unsigned __int32 PolyModeFrontPType : 3;
    unsigned __int32 PolyModeBackPType : 3;
    unsigned __int32 PolyOffsetFrontEnable : 1;
    unsigned __int32 PolyOffsetBackEnable : 1;
    unsigned __int32 PolyOffsetParaEnable : 1;
    unsigned __int32 : 1;
    unsigned __int32 MsaaEnable : 1;
    unsigned __int32 VtxWindowOffsetEnable : 1;
    unsigned __int32 : 2;
    unsigned __int32 ProvokingVtxLast : 1;
    unsigned __int32 PerspCorrDisable : 1;
    unsigned __int32 MultiPrimIbEnable : 1;
    unsigned __int32 : 1;
    unsigned __int32 QuadOrderEnable : 1;
    unsigned __int32 ScOneQuadPerClock : 1;
} _94AFD67874D6A703F84CC15A9C8F6192;

typedef union GPU_MODECONTROL { // DB types_members GPU_MODECONTROL
    _94AFD67874D6A703F84CC15A9C8F6192 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_MODECONTROL;
