#pragma once

/* recorded_animation_time_delta — the 2-bit time-delta encoding tag in a recorded-animation
 * event stream's header byte (event_byte & 3): how many ticks elapse before the event, and how
 * the count is stored. DB enum $44CC36BF4C54B8284BA80B81BDEF81C2. */
enum recorded_animation_time_delta
{
    _time_delta_zero = 0,   /* event fires on the same tick — no delta stored */
    _time_delta_one  = 1,   /* one tick — no delta stored */
    _time_delta_byte = 2,   /* delta is the next byte */
    _time_delta_word = 3,   /* delta is the next 16-bit word */
    NUMBER_OF_TIME_DELTAS = 4
};
