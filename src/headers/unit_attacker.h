#pragma once
/* unit_attacker — one entry of a unit_datum's recent-attackers list (16 bytes, DB size). */

typedef struct unit_attacker
{
    unsigned int game_time_stamp; /* 0x00 */
    float        damage_inflicted; /* 0x04 */
    int          object_index;     /* 0x08 */
    int          player_index;     /* 0x0C */
} unit_attacker; /* 16 bytes */
