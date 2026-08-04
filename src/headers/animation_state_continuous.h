#pragma once
/* animation_state_continuous — like animation_state but with a fractional (float) frame index (8 bytes). */

typedef struct animation_state_continuous
{
    __int16          index;       /* 0x0 — animation index within the graph, -1 = none */
    unsigned __int16 pad;         /* 0x2 */
    float            frame_index; /* 0x4 */
} animation_state_continuous;     /* 8 bytes */
