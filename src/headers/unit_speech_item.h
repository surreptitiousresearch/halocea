#pragma once

#include "ai_information_packet.h"

/* unit_speech_item @ 48 bytes — one queued AI speech/vocalization request handed to unit_speak. */
typedef struct unit_speech_item {
    __int16 priority;               /* 0x00 */
    __int16 vocalization_type;      /* 0x02 */
    int sound_definition_index;     /* 0x04 */
    __int16 delay_time;             /* 0x08 */
    __int16 ai_notification_delay;  /* 0x0A */
    __int16 pause_time;             /* 0x0C */
    unsigned __int16 pad;           /* 0x0E */
    ai_information_packet ai;        /* 0x10 */
} unit_speech_item;
