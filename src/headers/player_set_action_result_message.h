#pragma once
/* player_set_action_result_network_data — wire body for replicating a player action result (weapon
 * pickup/swap, seat entry) to client machines. Player/object/weapon indices are wire-translated before
 * packing. DB-verified via types_members player_set_action_result_network_data (20 bytes). */

#include <stdint.h>
#include "networked_set_action_result_type.h"

typedef struct player_set_action_result_network_data
{
    int     player_index;                     /* 0x00 — network-translated */
    networked_set_action_result_type action_result_type; /* 0x04 */
    int     action_object_index;              /* 0x08 — network-translated */
    int16_t action_result;                    /* 0x0C */
    int16_t action_seat_index;                /* 0x0E */
    int     weapon_to_drop_as_result_of_swap; /* 0x10 — network-translated */
} player_set_action_result_message; /* legacy local name for player_set_action_result_network_data */
