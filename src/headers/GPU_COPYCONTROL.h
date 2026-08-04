#pragma once

/* GPU_COPYCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_COPYCONTROL). */
typedef struct _02ECE00345890BD8590E5901D4E245C9 { // DB types_members _02ECE00345890BD8590E5901D4E245C9
    unsigned __int32 CopySrcSelect : 3;
    unsigned __int32 : 1;
    unsigned __int32 CopySampleSelect : 3;
    unsigned __int32 : 1;
    unsigned __int32 ColorClearEnable : 1;
    unsigned __int32 DepthClearEnable : 1;
    unsigned __int32 : 10;
    unsigned __int32 CopyCommand : 2;
} _02ECE00345890BD8590E5901D4E245C9;

typedef union GPU_COPYCONTROL { // DB types_members GPU_COPYCONTROL
    _02ECE00345890BD8590E5901D4E245C9 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_COPYCONTROL;
