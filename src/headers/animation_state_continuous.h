#pragma once
#include <stdint.h>
/* animation_state_continuous — like animation_state but with a fractional (float) frame index (8 bytes). */

typedef struct animation_state_continuous
{
    int16_t          index;       /* 0x0 — animation index within the graph, -1 = none */
    uint16_t pad;         /* 0x2 */
    float            frame_index; /* 0x4 */
} animation_state_continuous;     /* 8 bytes */
