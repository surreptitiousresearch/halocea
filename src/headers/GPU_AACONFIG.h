#pragma once
#include <stdint.h>

/* GPU_AACONFIG - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_AACONFIG). */
typedef struct _69FB5EC1060AAC04DB28CE0EF8F33FD2 { // DB types_members _69FB5EC1060AAC04DB28CE0EF8F33FD2
    uint32_t MsaaNumSamples : 3;
    uint32_t : 10;
    uint32_t MaxSampleDist : 4;
} _69FB5EC1060AAC04DB28CE0EF8F33FD2;

typedef union GPU_AACONFIG { // DB types_members GPU_AACONFIG
    _69FB5EC1060AAC04DB28CE0EF8F33FD2 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_AACONFIG;
