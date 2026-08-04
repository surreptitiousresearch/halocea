#include <stdint.h>
#include "headers/first_person_weapon.h"

first_person_weapon * first_person_weapon_get(int16_t local_player_index)
{
    return &first_person_weapons[local_player_index];
}
