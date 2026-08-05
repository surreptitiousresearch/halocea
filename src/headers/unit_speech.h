#pragma once
/* unit_speech — a unit_datum's dialogue/speech playback state (124 bytes, DB size). */

#include <stdint.h>
#include "unit_speech_item.h"

typedef struct unit_speech
{
    unit_speech_item current;            /* 0x00 */
    unit_speech_item queued;             /* 0x30 */
    int16_t damage_minor_decay_timer;    /* 0x60 */
    int16_t damage_minor_sounds;         /* 0x62 */
    int16_t damage_minor_timer;          /* 0x64 */
    int16_t damage_major_timer;          /* 0x66 */
    int last_speech_finished_time;       /* 0x68 */
    uint8_t played;              /* 0x6C */
    uint8_t notified_ai;         /* 0x6D */
    uint8_t finished;            /* 0x6E */
    uint8_t pad;                 /* 0x6F */
    int16_t pre_delay_timer;             /* 0x70 */
    int16_t sound_timer;                 /* 0x72 */
    int16_t ai_delay_timer;              /* 0x74 */
    int16_t post_delay_timer;            /* 0x76 */
    int impulse_sound_index;             /* 0x78 */
} unit_speech; /* 124 bytes */
