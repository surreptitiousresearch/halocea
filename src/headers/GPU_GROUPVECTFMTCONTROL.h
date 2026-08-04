#pragma once

/* GPU_GROUPVECTFMTCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_GROUPVECTFMTCONTROL). */
typedef struct _0B5F7FB2731F71913AC6525C5F302169 { // DB types_members _0B5F7FB2731F71913AC6525C5F302169
    unsigned __int32 XConv : 4;
    unsigned __int32 XOffset : 4;
    unsigned __int32 YConv : 4;
    unsigned __int32 YOffset : 4;
    unsigned __int32 ZConv : 4;
    unsigned __int32 ZOffset : 4;
    unsigned __int32 WConv : 4;
    unsigned __int32 WOffset : 4;
} _0B5F7FB2731F71913AC6525C5F302169;

typedef union GPU_GROUPVECTFMTCONTROL { // DB types_members GPU_GROUPVECTFMTCONTROL
    _0B5F7FB2731F71913AC6525C5F302169 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_GROUPVECTFMTCONTROL;
