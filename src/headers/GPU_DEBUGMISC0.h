#pragma once
#include <stdint.h>

/* GPU_DEBUGMISC0 - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_DEBUGMISC0). */
typedef struct _06FE1236073F1FBB77DD0B78F187A090 { // DB types_members _06FE1236073F1FBB77DD0B78F187A090
    uint32_t DbProgOn : 1;
    uint32_t : 3;
    uint32_t DbProbBreak : 1;
    uint32_t : 3;
    uint32_t DbProbAddr : 11;
    uint32_t : 5;
    uint32_t DbProbCount : 8;
} _06FE1236073F1FBB77DD0B78F187A090;

typedef union GPU_DEBUGMISC0 { // DB types_members GPU_DEBUGMISC0
    _06FE1236073F1FBB77DD0B78F187A090 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_DEBUGMISC0;
