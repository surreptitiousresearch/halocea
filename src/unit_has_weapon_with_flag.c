/* unit_has_weapon_with_flag @0x836CD340 — true if any of a unit's 4 weapon inventory slots holds a
 * weapon object whose weapon_definition.flags has the given bit set. Empty slots (index -1) count as a
 * match on the -1 sentinel and are skipped via the flag test short-circuit.
 *
 * DEVIATION: return is `uint8_t` (bool), not `int` — the caller masks r3 with `clrlwi ...,24` (keeps
 * low 8 bits) before testing, proving a byte-width result; matches the corpus `unit_*` predicate
 * convention. `flag` is `int` (r4 is consumed as a full 32-bit shift operand with no char narrowing). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/maximum_weapons_per_unit.h"
#include "headers/blam_data_globals.h"


uint8_t unit_has_weapon_with_flag(int unit_index, int flag)
{
    unit_datum *unit = (unit_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    int *weapon_slot = &unit->unit.weapon_object_indices[0];
    for ( int slot_count = 0; ; ++weapon_slot )
    {
        int weapon_index = *weapon_slot;
        if ( weapon_index != -1 )
        {
            weapon_datum *weapon = (weapon_datum *)
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
            weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
            if ( (definition->weapon.flags & (1 << flag)) != 0 )
                return 1;
        }
        if ( ++slot_count >= MAXIMUM_WEAPONS_PER_UNIT )
            return 0;
    }
}
