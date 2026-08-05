#pragma once

#include <stdint.h>
#include "ai_information_packet.h"

/* unit_speech_item @ 48 bytes — one queued AI speech/vocalization request handed to unit_speak. */
typedef struct unit_speech_item {
    int16_t priority;               /* 0x00 */
    int16_t vocalization_type;      /* 0x02 */
    int sound_definition_index;     /* 0x04 */
    int16_t delay_time;             /* 0x08 */
    int16_t ai_notification_delay;  /* 0x0A */
    int16_t pause_time;             /* 0x0C */
    uint16_t pad;           /* 0x0E */
    ai_information_packet ai;        /* 0x10 */
} unit_speech_item;
