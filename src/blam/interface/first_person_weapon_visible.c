/* first_person_weapon_visible @0x8369F038 */
#include <stdint.h>
#include "headers/first_person_weapon.h"

int first_person_weapon_visible(int16_t plr_idx)
{
    return first_person_weapons[plr_idx].visible;
}
