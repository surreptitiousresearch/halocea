#pragma once
/* encounter_definition — element of scenario.ai_encounters (176 bytes). Fully named per an
 * authoritative structure dump (C:\halo\headers_ref\encounter_definition.h, 2026-07-09) — resolves
 * respawn_time_lower_bound/upper_bound, previously cited as raw "encounter def floats +0x2C/+0x30". */

#include "tag_block.h"

typedef struct encounter_definition
{
    char      name[32];          /* 0x00 */
    unsigned int flags;          /* 0x20 — bit1=initially blind(0x2), bit2(0x4), bit3(0x8) (disasm-confirmed) */
    __int16   team_index;        /* 0x24 (disasm-confirmed lhz 0x24) */
    __int16   version;           /* 0x26 */
    __int16   searching;         /* 0x28 */
    __int16   manual_structure_bsp_reference_index; /* 0x2A */
    float     respawn_time_lower_bound;  /* 0x2C */
    float     respawn_time_upper_bound;  /* 0x30 */
    unsigned int unused[18];     /* 0x34 */
    unsigned __int16 pad2;       /* 0x7C */
    __int16   runtime_structure_bsp_reference_index; /* 0x7E */
    tag_block squads;            /* 0x80 — 232-byte elements, name at offset 0 */
    tag_block platoons;          /* 0x8C — 172-byte elements, name at offset 0 */
    tag_block firing_positions;  /* 0x98 */
    tag_block player_starting_locations; /* 0xA4 */
} encounter_definition;          /* 0xB0 = 176 bytes */
