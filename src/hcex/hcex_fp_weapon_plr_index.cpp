/* hcex_fp_weapon_plr_index @0x8369EEA8 — given a Blam object index, return which local-player first-person
 * weapon slot (0 or 1) currently holds that weapon object, or -1 if none does. first_person_weapons is the
 * Blam viewmodel array (one entry per local player). */

#include "../headers/first_person_weapon.h"

extern "C" int hcex_fp_weapon_plr_index(int object_index)
{
    int local_player_index;
    for (local_player_index = 0; local_player_index < 2; local_player_index = (short)(local_player_index + 1))
    {
        if (first_person_weapons[local_player_index].weapon_index == object_index)
            return local_player_index;
    }
    return -1;
}
