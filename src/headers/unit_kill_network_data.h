#pragma once
/* unit_kill_network_data — payload assembled by unit_damage_aftermath and handed to unit_kill_to_network
 * so a unit death/ping can be replicated to clients. Layout from the database (32 bytes). */

#include "real_vector2d.h"

typedef struct unit_kill_network_data
{
    int            unit_index;                      /* 0x00 */
    unsigned __int8 should_cause_ping;              /* 0x04 */
    unsigned __int8 died;                           /* 0x05 */
    unsigned __int8 feign_death;                    /* 0x06 */
    unsigned __int8 died_flying;                    /* 0x07 */
    unsigned __int8 ignore_hard_pings;              /* 0x08 */
    unsigned __int8 force_hard_pings;               /* 0x09 */
    unsigned __int8 should_alignment_vector_be_null;/* 0x0A */
    char           _pad0B;                          /* 0x0B */
    __int16        damage_part;                     /* 0x0C */
    char           _pad0E[2];                       /* 0x0E */
    float          angle;                           /* 0x10 */
    real_vector2d  alignment_vector;                /* 0x14 */
    int            respawn_timer;                   /* 0x1C */
} unit_kill_network_data;
