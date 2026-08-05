#pragma once
#include <stdint.h>
/* vehicle_info_s — a scenario vehicle placement's runtime remap info (DB types_members-confirmed). */

typedef struct vehicle_info_s
{
    unsigned int      scenario_datum_index; /* 0x00 */
    uint8_t   spawn;                /* 0x04 */
    unsigned char _pad0[1]; /* db-verified padding */
    uint16_t  spawn_flags;          /* 0x06 — per-game-engine "is default vehicle" bits */
} vehicle_info_s;
