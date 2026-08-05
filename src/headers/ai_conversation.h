#pragma once
/* ai_conversation — one scenario AI conversation definition (116 bytes), addressed via
 * scenario.ai_conversations. Full DB layout. */

#include <stdint.h>
#include "tag_block.h"

typedef struct ai_conversation
{
    char             name[32];            /* 0x00 */
    uint16_t flags;               /* 0x20 */
    uint16_t pad;                 /* 0x22 */
    float            trigger_dist;        /* 0x24 */
    float            run_to_player_dist;  /* 0x28 */
    unsigned int     unused[9];           /* 0x2C */
    tag_block        participants;        /* 0x50 — ai_conversation_participant[] */
    tag_block        lines;               /* 0x5C — ai_conversation_line[] */
    tag_block        unused_block;        /* 0x68 */
} ai_conversation;                        /* 116 bytes */
