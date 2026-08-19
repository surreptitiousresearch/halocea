/* unit_get_weapon_count @0x836CB4A8 — number of "real" weapons the unit carries: of the four weapon slots
 * (object index at word 190+slot), count those whose weapon tag does not have flag bit 0x10 set (that bit marks
 * weapons excluded from the count, e.g. flashlight/ability items). The weapon tag flags are word index 194 of
 * the weapon's tag instance data. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_definition_flags.h"
#include "headers/unit_datum.h"
#include "headers/maximum_weapons_per_unit.h"
#include "headers/blam_data_globals.h"


int16_t unit_get_weapon_count(int unit_index)
{
    int count = 0;
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    for ( int slot = 0; slot < MAXIMUM_WEAPONS_PER_UNIT; slot = (int16_t)(slot + 1) )
    {
        int weapon_object_index = unit->unit.weapon_object_indices[slot];
        if ( weapon_object_index != -1 )
        {
            object_datum *weapon_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_object_index)->datum;
            weapon_definition *weapon_tag = TAG_GET(weapon_definition, weapon_object->definition_index);
            if ( (weapon_tag->weapon.flags & (1u << _weapon_doesnt_count_toward_maximum_bit)) == 0 )   /* +776 */
                count = (int16_t)(count + 1);
        }
    }
    return count;
}
