#pragma once
#include <stdint.h>

/* GPU_GROUPPRIMTYPE - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_GROUPPRIMTYPE). */
typedef struct _88D1C91C7D60AD3B5EED5018C745811E { // DB types_members _88D1C91C7D60AD3B5EED5018C745811E
    uint32_t GroupPrimType : 4;
    uint32_t : 10;
    uint32_t GroupRetainOrder : 1;
    uint32_t GroupRetainQuads : 1;
    uint32_t GroupPrimOrder : 3;
} _88D1C91C7D60AD3B5EED5018C745811E;

typedef union GPU_GROUPPRIMTYPE { // DB types_members GPU_GROUPPRIMTYPE
    _88D1C91C7D60AD3B5EED5018C745811E __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_GROUPPRIMTYPE;
