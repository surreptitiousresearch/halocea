#pragma once

/* GPU_DEPTHCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_DEPTHCONTROL). */
typedef struct _58A3F58A4CC4288EF83802E375919710 { // DB types_members _58A3F58A4CC4288EF83802E375919710
    unsigned __int32 StencilEnable : 1;
    unsigned __int32 ZEnable : 1;
    unsigned __int32 ZWriteEnable : 1;
    unsigned __int32 : 1;
    unsigned __int32 ZFunc : 3;
    unsigned __int32 BackFaceEnable : 1;
    unsigned __int32 StencilFunc : 3;
    unsigned __int32 StencilFail : 3;
    unsigned __int32 StencilZPass : 3;
    unsigned __int32 StencilZFail : 3;
    unsigned __int32 StencilFuncBF : 3;
    unsigned __int32 StencilFailBF : 3;
    unsigned __int32 StencilZPassBF : 3;
    unsigned __int32 StencilZFailBF : 3;
} _58A3F58A4CC4288EF83802E375919710;

typedef union GPU_DEPTHCONTROL { // DB types_members GPU_DEPTHCONTROL
    _58A3F58A4CC4288EF83802E375919710 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_DEPTHCONTROL;
