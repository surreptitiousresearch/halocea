/* unit_handle_region_destroyed @0x836CCA70 — respond to one of a unit's damage regions being destroyed:
 * if the unit is not already dead, play the appropriate scream (head vs. limb, chosen from the region
 * damage flags). region_damage_flags bit 0x200 selects the "destroyed head" scream, else "destroyed limb". */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/object_region_flags.h"
#include "headers/unit_scream_type.h"
#include "headers/blam_data_globals.h"

extern int unit_scream(int unit_index, int16_t scream_type);

void unit_handle_region_destroyed(int unit_index, int16_t region_index, unsigned int region_damage_flags)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    if ((unit->object.damage_flags & (1u << _object_dead_bit)) == 0)
        unit_scream(unit_index,
                    (region_damage_flags & (1u << _object_region_head_destroyed_scream_bit)) ? _unit_scream_destroyed_head : _unit_scream_destroyed_limb);
}
