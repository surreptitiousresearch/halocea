#pragma once

/* GPU_CONTEXTMISC - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_CONTEXTMISC). */
typedef struct _8CE5B508FFC7A2118BA8518A4C89AB9F { // DB types_members _8CE5B508FFC7A2118BA8518A4C89AB9F
    unsigned __int32 InstPredOptimize : 1;
    unsigned __int32 OutputScreenXY : 1;
    unsigned __int32 SampleControl : 2;
    unsigned __int32 : 4;
    unsigned __int32 ParamGenPos : 8;
    unsigned __int32 PerfCounterRef : 1;
    unsigned __int32 YieldOptimize : 1;
    unsigned __int32 TxCacheSelect : 1;
} _8CE5B508FFC7A2118BA8518A4C89AB9F;

typedef union GPU_CONTEXTMISC { // DB types_members GPU_CONTEXTMISC
    _8CE5B508FFC7A2118BA8518A4C89AB9F __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_CONTEXTMISC;
