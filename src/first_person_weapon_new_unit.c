#include <stdint.h>
#include "headers/first_person_weapon.h"
#include "headers/blam_data_globals.h"

extern void first_person_weapon_switch_weapons(int16_t local_player_index);

void first_person_weapon_new_unit(int16_t local_player_index, int unit_index)
{
    first_person_weapon *fp = &first_person_weapons[local_player_index];
    fp->rendered = 0;
    fp->unit_index = unit_index;
    first_person_weapon_switch_weapons(local_player_index);
}
