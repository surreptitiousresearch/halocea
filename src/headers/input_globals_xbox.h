#pragma once
/* input_globals_xbox — raw per-frame input device state (input.c, Xbox backend). Layout from the database. */

#include "gamepad_state.h"
#include "key_stroke.h"

typedef struct input_globals_xbox
{
    gamepad_state   gamepads[4];             /* 0x000 */
    unsigned __int8 key_ticks[109];          /* 0x0A0 */
    unsigned __int8 key_latches[109];        /* 0x10D */
    __int16         buffered_key_read_index; /* 0x17A */
    __int16         buffered_key_write_index;/* 0x17C */
    key_stroke      buffered_keys[64];       /* 0x17E */
} input_globals_xbox;                        /* 638 bytes */
