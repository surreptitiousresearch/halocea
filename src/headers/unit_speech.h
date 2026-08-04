#pragma once
/* unit_speech — a unit_datum's dialogue/speech playback state (124 bytes, DB size). */

#include "unit_speech_item.h"

typedef struct unit_speech
{
    unit_speech_item current;            /* 0x00 */
    unit_speech_item queued;             /* 0x30 */
    __int16 damage_minor_decay_timer;    /* 0x60 */
    __int16 damage_minor_sounds;         /* 0x62 */
    __int16 damage_minor_timer;          /* 0x64 */
    __int16 damage_major_timer;          /* 0x66 */
    int last_speech_finished_time;       /* 0x68 */
    unsigned __int8 played;              /* 0x6C */
    unsigned __int8 notified_ai;         /* 0x6D */
    unsigned __int8 finished;            /* 0x6E */
    unsigned __int8 pad;                 /* 0x6F */
    __int16 pre_delay_timer;             /* 0x70 */
    __int16 sound_timer;                 /* 0x72 */
    __int16 ai_delay_timer;              /* 0x74 */
    __int16 post_delay_timer;            /* 0x76 */
    int impulse_sound_index;             /* 0x78 */
} unit_speech; /* 124 bytes */
