#pragma once

/* GPU_CONST - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_CONST). */
typedef struct _FF7A386D719D0A42A365ABC5F186FFA8 { // DB types_members _FF7A386D719D0A42A365ABC5F186FFA8
    unsigned __int32 Base : 9;
    unsigned __int32 : 3;
    unsigned __int32 Size : 9;
} _FF7A386D719D0A42A365ABC5F186FFA8;

typedef union GPU_CONST { // DB types_members GPU_CONST
    _FF7A386D719D0A42A365ABC5F186FFA8 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_CONST;
