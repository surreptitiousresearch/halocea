/* unit_integrated_night_vision_is_active @ 0x836D1A78 — true when the unit's currently-readied weapon
 * has its integrated light (night vision) enabled. Bails early if the unit has no night-vision capability
 * (unit.current_zoom_level == -1/255). Reads the unit's current weapon slot, looks up the weapon object
 * index, and tests the weapon definition's flag bit 0x4000 (definition word[194]). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_definition_flags.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


uint8_t unit_integrated_night_vision_is_active(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    if ( (uint8_t)unit->unit.current_zoom_level == 255 )
        return 0;

    int weapon_slot = unit->unit.current_weapon_index;
    if ( weapon_slot == -1 )
        return 0;

    int weapon_object_index = unit->unit.weapon_object_indices[weapon_slot];
    if ( weapon_object_index == -1 )
        return 0;

    /* was an int/(int *) pun over the datum pointer; typed as object_datum per the sibling unit fns */
    object_datum *weapon_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_object_index)->datum;
    return (TAG_GET(weapon_definition, weapon_object->definition_index)->weapon.flags & (1u << _weapon_enables_unit_integrated_night_vision_bit)) != 0;   /* +776 */
}
