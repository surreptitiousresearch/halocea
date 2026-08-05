#pragma once
#include <stdint.h>

/* GPU_COPYCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_COPYCONTROL). */
typedef struct _02ECE00345890BD8590E5901D4E245C9 { // DB types_members _02ECE00345890BD8590E5901D4E245C9
    uint32_t CopySrcSelect : 3;
    uint32_t : 1;
    uint32_t CopySampleSelect : 3;
    uint32_t : 1;
    uint32_t ColorClearEnable : 1;
    uint32_t DepthClearEnable : 1;
    uint32_t : 10;
    uint32_t CopyCommand : 2;
} _02ECE00345890BD8590E5901D4E245C9;

typedef union GPU_COPYCONTROL { // DB types_members GPU_COPYCONTROL
    _02ECE00345890BD8590E5901D4E245C9 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_COPYCONTROL;
