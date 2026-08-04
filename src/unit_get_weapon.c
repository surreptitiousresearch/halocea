/* unit_get_weapon @0x836CC238 — the object index of the weapon in the given slot of a unit. Reads
 * unit.weapon_object_indices[index] (word index 190 = 0x2F8 from the datum base). Returns -1 for an
 * invalid slot. The unit argument is the unit data pointer. */

#include <stdint.h>
#include "headers/unit_datum.h"

int unit_get_weapon(unit_datum *unit, int16_t index)
{
    if ( index == -1 )
        return -1;
    return unit->unit.weapon_object_indices[index];
}
