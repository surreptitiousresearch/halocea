/* global_speech_queue_times @ 0x821263DC - .rdata read-only table, 44 bytes.
 * unit_speech_priority -> how long (SECONDS) a request of that priority may sit queued behind the
 * line a unit is currently speaking. unit_test_speech.c reads it three ways: 0 means "never
 * queue", 3.4028235e38f (FLT_MAX, image word 0x7F7FFFFF) means "always queue", and anything else
 * is compared as ticks against post_delay_timer + sound_timer (`* 30.0f`, line 95).
 *
 * 44 bytes / 4 == 11 == NUMBER_OF_UNIT_SPEECH_PRIORITIES - exactly the index domain, no padding.
 * Element type float confirmed by the load in unit_test_speech:
 *   837BA864  lis   r11, global_speech_queue_times@ha
 *   837BA868  slwi  r10, r9, 2
 *   837BA874  lfsx  f0, r10, r11
 *
 * raw -> resolved:
 *   +0x00 0x00000000 -> 0.0f           [_unit_speech_none]
 *   +0x04 0x00000000 -> 0.0f           [_unit_speech_idle]
 *   +0x08 0x00000000 -> 0.0f           [_unit_speech_pain]
 *   +0x0C 0x3FC00000 -> 1.5f           [_unit_speech_talk]
 *   +0x10 0x40400000 -> 3.0f           [_unit_speech_communicate]
 *   +0x14 0x40800000 -> 4.0f           [_unit_speech_shout]
 *   +0x18 0x7F7FFFFF -> 3.4028235e38f  [_unit_speech_scripted]
 *   +0x1C 0x40400000 -> 3.0f           [_unit_speech_involuntary]
 *   +0x20 0x40400000 -> 3.0f           [_unit_speech_exclamation]
 *   +0x24 0x40400000 -> 3.0f           [_unit_speech_scream]
 *   +0x28 0x40400000 -> 3.0f           [_unit_speech_death]
 */
#include "../headers/unit_speech_priority.h"

float global_speech_queue_times[NUMBER_OF_UNIT_SPEECH_PRIORITIES] =
{
    0.0f,            /* [_unit_speech_none] */
    0.0f,            /* [_unit_speech_idle] */
    0.0f,            /* [_unit_speech_pain] */
    1.5f,            /* [_unit_speech_talk] */
    3.0f,            /* [_unit_speech_communicate] */
    4.0f,            /* [_unit_speech_shout] */
    3.4028235e38f,   /* [_unit_speech_scripted] */
    3.0f,            /* [_unit_speech_involuntary] */
    3.0f,            /* [_unit_speech_exclamation] */
    3.0f,            /* [_unit_speech_scream] */
    3.0f,            /* [_unit_speech_death] */
};
