#pragma once
/* vehicle_info_s — a scenario vehicle placement's runtime remap info (DB types_members-confirmed). */

typedef struct vehicle_info_s
{
    unsigned int      scenario_datum_index; /* 0x00 */
    unsigned __int8   spawn;                /* 0x04 */
    unsigned char _pad0[1]; /* db-verified padding */
    unsigned __int16  spawn_flags;          /* 0x06 — per-game-engine "is default vehicle" bits */
} vehicle_info_s;
