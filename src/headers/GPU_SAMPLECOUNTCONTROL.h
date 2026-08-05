#pragma once
#include <stdint.h>

/* GPU_SAMPLECOUNTCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_SAMPLECOUNTCONTROL). */
typedef struct _FBD7574F7E4129E548765D743E467685 { // DB types_members _FBD7574F7E4129E548765D743E467685
    uint32_t ResetSampleCount : 1;
    uint32_t CopySampleCount : 1;
} _FBD7574F7E4129E548765D743E467685;

typedef union GPU_SAMPLECOUNTCONTROL { // DB types_members GPU_SAMPLECOUNTCONTROL
    _FBD7574F7E4129E548765D743E467685 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_SAMPLECOUNTCONTROL;
