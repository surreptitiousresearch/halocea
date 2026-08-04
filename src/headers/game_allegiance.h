#pragma once
/* game_allegiance — a single AI team-alliance record and the incident state that can
 * break it (friendly fire etc.) (game_allegiance.c). The composite globals block that
 * embeds these lives in struct_game_allegiance_globals.h. */

typedef struct game_allegiance
{
    __int16         team1_index;                 /* 0x00 */
    __int16         team2_index;                 /* 0x02 */
    __int16         incident_threshold;          /* 0x04 */
    __int16         incident_decay_time;         /* 0x06 */
    unsigned __int8 team1_suspicious;            /* 0x08 */
    unsigned __int8 team2_suspicious;            /* 0x09 */
    unsigned __int8 currently_broken;            /* 0x0A */
    unsigned __int8 status_changed;              /* 0x0B */
    unsigned __int8 requires_communication;      /* 0x0C */
    unsigned char _pad0[1]; /* db-verified padding */
    __int16         current_incidents;           /* 0x0E */
    __int16         current_incident_decay_time; /* 0x10 */
} game_allegiance;                               /* 18 bytes */

/* Composite globals block embeds game_allegiance (defined above); include after the
 * leaf definition to preserve the type set this header historically exposed. */
#include "struct_game_allegiance_globals.h"
