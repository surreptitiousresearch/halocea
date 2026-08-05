#pragma once
#include <stdint.h>
/* key_stroke — one decoded keyboard event (DB layout, 4 bytes). */

typedef struct key_stroke
{
    uint8_t modifier_flags; /* 0x0 */
    uint8_t ascii_code;     /* 0x1 */
    int16_t         key_code;       /* 0x2 */
} key_stroke;                       /* 4 bytes */
