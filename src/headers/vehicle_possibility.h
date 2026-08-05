#pragma once
/* vehicle_possibility — stack-local candidate record used by action_obey_command_begin's
 * _ai_atom_vehicle case (and sorted by vehicle_possibility_qsort on the leading float).
 * NOT a DB type: layout evidenced by the disasm store pattern (8-byte records, float
 * distance_sq at +0 used as the qsort key, int vehicle_index at +4). */

typedef struct vehicle_possibility
{
    float distance_sq;   /* 0x00 — sort key */
    int   vehicle_index; /* 0x04 */
} vehicle_possibility;   /* 8 bytes */
