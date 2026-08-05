#pragma once
#include <stdint.h>

/* GPU_MODECONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_MODECONTROL). */
typedef struct _94AFD67874D6A703F84CC15A9C8F6192 { // DB types_members _94AFD67874D6A703F84CC15A9C8F6192
    uint32_t CullMode : 3;
    uint32_t PolyMode : 2;
    uint32_t PolyModeFrontPType : 3;
    uint32_t PolyModeBackPType : 3;
    uint32_t PolyOffsetFrontEnable : 1;
    uint32_t PolyOffsetBackEnable : 1;
    uint32_t PolyOffsetParaEnable : 1;
    uint32_t : 1;
    uint32_t MsaaEnable : 1;
    uint32_t VtxWindowOffsetEnable : 1;
    uint32_t : 2;
    uint32_t ProvokingVtxLast : 1;
    uint32_t PerspCorrDisable : 1;
    uint32_t MultiPrimIbEnable : 1;
    uint32_t : 1;
    uint32_t QuadOrderEnable : 1;
    uint32_t ScOneQuadPerClock : 1;
} _94AFD67874D6A703F84CC15A9C8F6192;

typedef union GPU_MODECONTROL { // DB types_members GPU_MODECONTROL
    _94AFD67874D6A703F84CC15A9C8F6192 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_MODECONTROL;
