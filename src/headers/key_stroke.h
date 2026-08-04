#pragma once
/* key_stroke — one decoded keyboard event (DB layout, 4 bytes). */

typedef struct key_stroke
{
    unsigned __int8 modifier_flags; /* 0x0 */
    unsigned __int8 ascii_code;     /* 0x1 */
    __int16         key_code;       /* 0x2 */
} key_stroke;                       /* 4 bytes */
