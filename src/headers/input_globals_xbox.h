#pragma once
/* input_globals_xbox — raw per-frame input device state (input.c, Xbox backend). Layout from the database. */

#include <stdint.h>
#include "gamepad_state.h"
#include "key_stroke.h"

typedef struct input_globals_xbox
{
    gamepad_state   gamepads[4];             /* 0x000 */
    uint8_t key_ticks[109];          /* 0x0A0 */
    uint8_t key_latches[109];        /* 0x10D */
    int16_t         buffered_key_read_index; /* 0x17A */
    int16_t         buffered_key_write_index;/* 0x17C */
    key_stroke      buffered_keys[64];       /* 0x17E */
} input_globals_xbox;                        /* 638 bytes */
