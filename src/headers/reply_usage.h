#pragma once
/* reply_usage — one row of the global reply table (global_reply_table): given an original line's vocalization
 * (and damage category), describes a reply another actor may voice. Terminated by an entry whose
 * original_vocalization_type is 0xFFFF. Layout from the database. */

#include "ai_information_packet.h"

typedef struct reply_usage
{
    __int16          original_vocalization_type;  /* 0x00 */
    __int16          original_damage_category;    /* 0x02 */
    __int16          protagonist_type;            /* 0x04 */
    __int16          vocalization_type;           /* 0x06 */
    __int16          animation_type;              /* 0x08 */
    __int16          communication_priority;      /* 0x0A */
    unsigned __int16 flags;                       /* 0x0C */
    unsigned __int16 _pad0E;                      /* 0x0E */
    float            chance;                      /* 0x10 */
    float            player_chance;               /* 0x14 */
    float            delay_time;                  /* 0x18 */
    float            repeat_delay;                /* 0x1C */
    /* args: unit_index, ai_information, reply_actor_index */
    unsigned __int8  (*reply_filter)(int, ai_information_packet *, int);  /* 0x20 */
} reply_usage;                                    /* 0x24 */
