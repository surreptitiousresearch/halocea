#pragma once
/* ai_conversation_line — one line in a scenario AI conversation definition (124 bytes), addressed via
 * ai_conversation.lines. Full DB layout. */

#include <stdint.h>
#include "tag_reference.h"

typedef struct ai_conversation_line
{
    uint16_t flags;                     /* 0x00 */
    int16_t          participant_index;         /* 0x02 — speaker participant */
    int16_t          address_type;              /* 0x04 — 1 = triggering player, 2 = another participant */
    int16_t          address_participant_index; /* 0x06 — listener participant when address_type == 2 */
    unsigned int     unused;                    /* 0x08 */
    float            delay_time;                /* 0x0C — seconds */
    unsigned int     unused2[3];                /* 0x10 */
    tag_reference    dialogue[6];               /* 0x1C — sound tag per dialogue variant */
} ai_conversation_line;                         /* 124 bytes */
