#pragma once

/* GPU_GROUPPRIMTYPE - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_GROUPPRIMTYPE). */
typedef struct _88D1C91C7D60AD3B5EED5018C745811E { // DB types_members _88D1C91C7D60AD3B5EED5018C745811E
    unsigned __int32 GroupPrimType : 4;
    unsigned __int32 : 10;
    unsigned __int32 GroupRetainOrder : 1;
    unsigned __int32 GroupRetainQuads : 1;
    unsigned __int32 GroupPrimOrder : 3;
} _88D1C91C7D60AD3B5EED5018C745811E;

typedef union GPU_GROUPPRIMTYPE { // DB types_members GPU_GROUPPRIMTYPE
    _88D1C91C7D60AD3B5EED5018C745811E __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_GROUPPRIMTYPE;
