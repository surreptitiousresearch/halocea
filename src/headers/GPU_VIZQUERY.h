#pragma once
#include <stdint.h>

/* GPU_VIZQUERY - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_VIZQUERY). */
typedef struct _28A3CE7FC9E89F58D9643E379F323C10 { // DB types_members _28A3CE7FC9E89F58D9643E379F323C10
    uint32_t VizQueryEnable : 1;
    uint32_t VizQueryId : 6;
    uint32_t KillPixPostHiZ : 1;
    uint32_t KillPixPostDetailMask : 1;
} _28A3CE7FC9E89F58D9643E379F323C10;

typedef union GPU_VIZQUERY { // DB types_members GPU_VIZQUERY
    _28A3CE7FC9E89F58D9643E379F323C10 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_VIZQUERY;
