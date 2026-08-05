#pragma once

/* ai_information_packet @ 32 bytes — the AI communication/look/information payload embedded in a
 * unit_speech_item. look_data wraps the DB anonymous union _794E98B08E6C549F96497F8BA3FB527A (named
 * ___u0, 4 bytes). information_data is the shared 8-byte ai_information_data type (see
 * ai_information_data.h) — do not re-typedef it locally. $-name kept verbatim (MSVC extension),
 * matching the mtlPARAM.h convention. */

#include <stdint.h>
#include "ai_information_data.h"
#include "ai_unit_information_look_data.h"
#include "ai_object_information_look_data.h"

/* DB anonymous union _794E98B08E6C549F96497F8BA3FB527A (4 bytes) */
typedef union _794E98B08E6C549F96497F8BA3FB527A
{
    ai_unit_information_look_data   unit;    /* speaking-unit variant used by ai_conversation_line_perform */
    ai_object_information_look_data object;
} _794E98B08E6C549F96497F8BA3FB527A;

typedef struct ai_information_look_data {
    _794E98B08E6C549F96497F8BA3FB527A ___u0;  /* 0x00 */
} ai_information_look_data;

typedef struct ai_information_packet {
    int target_unit_index;                 /* 0x00 */
    int16_t communication_type;            /* 0x04 */
    int16_t dialogue_type_index;            /* 0x06 */
    int16_t damage_category;                /* 0x08 */
    uint8_t updated_dialogue_timers; /* 0x0A */
    uint8_t pad_b;                  /* 0x0B */
    int16_t look_priority;                  /* 0x0C */
    int16_t look_type;                      /* 0x0E */
    ai_information_look_data look_data;     /* 0x10 */
    int16_t information_type;               /* 0x14 */
    int16_t pad_16;                         /* 0x16 */
    ai_information_data information_data;    /* 0x18 (8 bytes, ends at 0x20 = 32) */
} ai_information_packet;
