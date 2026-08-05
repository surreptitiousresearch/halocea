#pragma once
#include <stdint.h>

/* GPU_OUTDEALLOCCONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_OUTDEALLOCCONTROL). */
typedef struct _ED982AC2C35D828DEC833A025400A5DC { // DB types_members _ED982AC2C35D828DEC833A025400A5DC
    uint32_t DeallocDist : 7;
} _ED982AC2C35D828DEC833A025400A5DC;

typedef union GPU_OUTDEALLOCCONTROL { // DB types_members GPU_OUTDEALLOCCONTROL
    _ED982AC2C35D828DEC833A025400A5DC __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_OUTDEALLOCCONTROL;
