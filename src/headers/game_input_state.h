#pragma once
#include <stdint.h>
/* game_input_state — abstracted per-player input for one frame, produced by the input-abstraction layer
 * (input_abstraction_get_input_state). `buttons` holds per-button press durations in ticks (0 = up).
 * Layout from the database. */

typedef struct game_input_state
{
    uint8_t buttons[13];        /* 0x00 — press durations in ticks */
    char            _pad[3];            /* 0x0D */
    float           forward_movement;   /* 0x10 */
    float           strafe;             /* 0x14 */
    float           yaw;                /* 0x18 */
    float           pitch;              /* 0x1C */
    uint8_t look_use_fancy;     /* 0x20 */
} game_input_state;                     /* 36 bytes */
