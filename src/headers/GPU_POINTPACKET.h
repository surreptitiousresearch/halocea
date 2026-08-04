#pragma once

/* GPU_POINTPACKET - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_POINTPACKET). */
typedef struct GPU_POINTPACKET { // DB types_members GPU_POINTPACKET
    float PolyOffsetFrontScale; /* 0x00 */
    float PolyOffsetFrontOffset; /* 0x04 */
    float PolyOffsetBackScale; /* 0x08 */
    float PolyOffsetBackOffset; /* 0x0C */
    float PointXRad; /* 0x10 */
    float PointYRad; /* 0x14 */
    float PointConstantSize; /* 0x18 */
    float PointCullRad; /* 0x1C */
} GPU_POINTPACKET;
