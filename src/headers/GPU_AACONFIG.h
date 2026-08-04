#pragma once

/* GPU_AACONFIG - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_AACONFIG). */
typedef struct _69FB5EC1060AAC04DB28CE0EF8F33FD2 { // DB types_members _69FB5EC1060AAC04DB28CE0EF8F33FD2
    unsigned __int32 MsaaNumSamples : 3;
    unsigned __int32 : 10;
    unsigned __int32 MaxSampleDist : 4;
} _69FB5EC1060AAC04DB28CE0EF8F33FD2;

typedef union GPU_AACONFIG { // DB types_members GPU_AACONFIG
    _69FB5EC1060AAC04DB28CE0EF8F33FD2 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_AACONFIG;
