#pragma once
/* player_effect_start_network_data — wire body for replicating a player-effect start (camera shake /
 * forced controller response) to clients. DB-verified via types_members player_effect_start_network_data. */

#include "real_vector3d.h"

typedef struct player_effect_start_network_data
{
    int           damage_definition_index;    /* 0x00 */
    int           damage_owner_object_index;  /* 0x04 (network-translated) */
    unsigned int  damage_flags;               /* 0x08 */
    real_vector3d direction;                  /* 0x0C */
    float         scale;                      /* 0x18 */
    float         total_damage;               /* 0x1C */
} player_effect_start_message; /* legacy local name for player_effect_start_network_data */
