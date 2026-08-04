#pragma once

/* GPU_COLORINFO - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_COLORINFO). */
typedef struct _1FF51BE4193A85843706EDD3EF9D36C0 { // DB types_members _1FF51BE4193A85843706EDD3EF9D36C0
    unsigned __int32 ColorBase : 12;
    unsigned __int32 : 4;
    unsigned __int32 ColorFormat : 4;
    unsigned __int32 ColorExpBias : 6;
} _1FF51BE4193A85843706EDD3EF9D36C0;

typedef union GPU_COLORINFO { // DB types_members GPU_COLORINFO
    _1FF51BE4193A85843706EDD3EF9D36C0 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_COLORINFO;
