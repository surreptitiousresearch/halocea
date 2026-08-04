#pragma once
/* gamepad_state — decoded controller state. `buttons` and `analog_buttons` hold per-control press
 * durations (frame counts), not booleans, so code can detect held/repeat. `sticks[0]` is the left
 * thumbstick, `sticks[1]` the right. Layout from the database. */

#include "point2d.h"

typedef struct gamepad_state
{
    unsigned __int8 analog_buttons[8];           /* 0x00 */
    unsigned __int8 analog_button_thresholds[8]; /* 0x08 */
    unsigned __int8 buttons[16];                 /* 0x10 — press durations in ticks */
    point2d         sticks[2];                   /* 0x20 — [0]=left, [1]=right */
} gamepad_state;                                 /* 40 bytes */
