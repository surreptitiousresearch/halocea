#pragma once
#include <stdint.h>

/* GPU_CONTEXTMISC - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_CONTEXTMISC). */
typedef struct _8CE5B508FFC7A2118BA8518A4C89AB9F { // DB types_members _8CE5B508FFC7A2118BA8518A4C89AB9F
    uint32_t InstPredOptimize : 1;
    uint32_t OutputScreenXY : 1;
    uint32_t SampleControl : 2;
    uint32_t : 4;
    uint32_t ParamGenPos : 8;
    uint32_t PerfCounterRef : 1;
    uint32_t YieldOptimize : 1;
    uint32_t TxCacheSelect : 1;
} _8CE5B508FFC7A2118BA8518A4C89AB9F;

typedef union GPU_CONTEXTMISC { // DB types_members GPU_CONTEXTMISC
    _8CE5B508FFC7A2118BA8518A4C89AB9F __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_CONTEXTMISC;
