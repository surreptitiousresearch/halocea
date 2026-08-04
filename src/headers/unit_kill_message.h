#pragma once
/* unit_kill_network_data — stateless message-delta decode target for the "unit kill" network message
 * (32 bytes). DB-verified via types_members unit_kill_network_data. */

#include "real_vector2d.h"

typedef struct unit_kill_network_data
{
    int             unit_index;                       /* 0x00 — translated object index */
    unsigned __int8 should_cause_ping;                /* 0x04 — gates the unit_ping_animation call */
    unsigned __int8 died;                             /* 0x05 */
    unsigned __int8 feign_death;                      /* 0x06 — also unit_died_client's "feigned" arg */
    unsigned __int8 died_flying;                      /* 0x07 */
    unsigned __int8 ignore_hard_pings;                /* 0x08 */
    unsigned __int8 force_hard_pings;                 /* 0x09 */
    unsigned __int8 should_alignment_vector_be_null;  /* 0x0A — gates whether alignment_vector is passed */
    unsigned char   _pad0B;                           /* 0x0B */
    __int16         damage_part;                      /* 0x0C */
    unsigned char   _pad0E[2];                        /* 0x0E */
    float           angle;                            /* 0x10 */
    real_vector2d   alignment_vector;                 /* 0x14 */
    int             respawn_timer;                    /* 0x1C — written to the killer's player_datum.respawn_timer */
} unit_kill_message; /* 0x20 bytes — legacy local name for unit_kill_network_data */
