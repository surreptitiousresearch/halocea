/* first_person_weapon_get_node_matrix @0x8369F260 — the posed world matrix for one node of a local
 * player's first-person weapon viewmodel. */

#include "headers/first_person_weapon.h"
#include "headers/real_matrix4x3.h"
#include <stdint.h>

real_matrix4x3 *first_person_weapon_get_node_matrix(int16_t local_player_index, int16_t node_index)
{
    return &first_person_weapons[local_player_index].node_matrices[node_index];
}
