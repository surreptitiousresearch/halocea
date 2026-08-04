#pragma once

/* GPU_MPASSPSCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_MPASSPSCONTROL). */
typedef struct _61A6C8188506F292C0603A5F49F70464 { // DB types_members _61A6C8188506F292C0603A5F49F70464
    unsigned __int32 MPassPixVecPerPass : 20;
    unsigned __int32 : 11;
    unsigned __int32 MPassPsEnable : 1;
} _61A6C8188506F292C0603A5F49F70464;

typedef union GPU_MPASSPSCONTROL { // DB types_members GPU_MPASSPSCONTROL
    _61A6C8188506F292C0603A5F49F70464 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_MPASSPSCONTROL;
