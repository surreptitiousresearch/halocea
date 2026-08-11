#pragma once
#include <stdint.h>
/* game_allegiance — a single AI team-alliance record and the incident state that can
 * break it (friendly fire etc.) (game_allegiance.c). The composite globals block that
 * embeds these lives in struct_game_allegiance_globals.h. */

typedef struct game_allegiance
{
    int16_t         team1_index;                 /* 0x00 */
    int16_t         team2_index;                 /* 0x02 */
    int16_t         incident_threshold;          /* 0x04 */
    int16_t         incident_decay_time;         /* 0x06 */
    uint8_t team1_suspicious;            /* 0x08 */
    uint8_t team2_suspicious;            /* 0x09 */
    uint8_t currently_broken;            /* 0x0A */
    uint8_t status_changed;              /* 0x0B */
    uint8_t requires_communication;      /* 0x0C */
    unsigned char _pad0[1]; /* db-verified padding */
    int16_t         current_incidents;           /* 0x0E */
    int16_t         current_incident_decay_time; /* 0x10 */
} game_allegiance;                               /* 18 bytes */

/* Composite globals block embeds game_allegiance (defined above); include after the
 * leaf definition to preserve the type set this header historically exposed. */
#include "struct_game_allegiance_globals.h"
