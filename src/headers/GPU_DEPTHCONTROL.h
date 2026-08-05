#pragma once
#include <stdint.h>

/* GPU_DEPTHCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_DEPTHCONTROL). */
typedef struct _58A3F58A4CC4288EF83802E375919710 { // DB types_members _58A3F58A4CC4288EF83802E375919710
    uint32_t StencilEnable : 1;
    uint32_t ZEnable : 1;
    uint32_t ZWriteEnable : 1;
    uint32_t : 1;
    uint32_t ZFunc : 3;
    uint32_t BackFaceEnable : 1;
    uint32_t StencilFunc : 3;
    uint32_t StencilFail : 3;
    uint32_t StencilZPass : 3;
    uint32_t StencilZFail : 3;
    uint32_t StencilFuncBF : 3;
    uint32_t StencilFailBF : 3;
    uint32_t StencilZPassBF : 3;
    uint32_t StencilZFailBF : 3;
} _58A3F58A4CC4288EF83802E375919710;

typedef union GPU_DEPTHCONTROL { // DB types_members GPU_DEPTHCONTROL
    _58A3F58A4CC4288EF83802E375919710 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_DEPTHCONTROL;
