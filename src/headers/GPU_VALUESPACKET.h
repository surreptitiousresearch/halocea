#pragma once

/* GPU_VALUESPACKET - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_VALUESPACKET). */
#include "GPU_COLORMASK.h"
#include "GPU_STENCILREFMASK.h"

typedef struct GPU_VALUESPACKET { // DB types_members GPU_VALUESPACKET
    unsigned int MaxVtxIndx; /* 0x00 */
    unsigned int MinVtxIndx; /* 0x04 */
    unsigned int IndxOffset; /* 0x08 */
    unsigned int MultiPrimIbResetIndx; /* 0x0C */
    GPU_COLORMASK ColorMask; /* 0x10 */
    float BlendRed; /* 0x14 */
    float BlendGreen; /* 0x18 */
    float BlendBlue; /* 0x1C */
    float BlendAlpha; /* 0x20 */
    unsigned int Unused[3]; /* 0x24 */
    GPU_STENCILREFMASK StencilRefMaskBF; /* 0x30 */
    GPU_STENCILREFMASK StencilRefMask; /* 0x34 */
    float AlphaRef; /* 0x38 */
    float VportXScale; /* 0x3C */
    float VportXOffset; /* 0x40 */
    float VportYScale; /* 0x44 */
    float VportYOffset; /* 0x48 */
    float VportZScale; /* 0x4C */
    float VportZOffset; /* 0x50 */
} GPU_VALUESPACKET;
