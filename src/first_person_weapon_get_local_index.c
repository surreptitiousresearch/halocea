/* first_person_weapon_get_local_index @0x8369EEF0 — map a weapon object index to the local first-person
 * weapon slot (0 or 1) that is currently rendering it. Returns -1 if no visible first-person weapon
 * holds the given object. */

#include "headers/first_person_weapon.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


int16_t first_person_weapon_get_local_index(int object_index)
{
    int i;
    for ( i = 0; i < 2; i = (int16_t)(i + 1) )  /* extsh r10,r11 — 16-bit loop counter */
    {
        first_person_weapon *fpw = &first_person_weapons[i];
        if ( fpw->weapon_index == object_index && fpw->visible )
            break;
    }
    if ( i == 2 )
        return -1;
    return (int16_t)i;  /* extsh r3,r10 */
}
