/* first_person_weapon_get_weapon_index @0x8369F288 — the weapon object index currently posed in a local
 * player's first-person view. */

#include "headers/first_person_weapon.h"
#include <stdint.h>

int first_person_weapon_get_weapon_index(int16_t local_player_index)
{
    return first_person_weapons[local_player_index].weapon_index;
}
