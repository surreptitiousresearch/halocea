/* weapon_rotate_zoom_level @ 0x836D9150 — advance a weapon's zoom level by one notch, wrapping back to
 * un-zoomed (-1) past the last level. Returns the unchanged level when the weapon has magazines and
 * magazine 0 is in state 1 (its toggle is handled elsewhere).
 *
 * Attested int16_t return (callee extsh r3 @0x836D91EC before blr; caller sth) and int16_t zoom_level
 * (extsh r4 @0x836D91C8).
 *
 * Deviation: restored the TAG_INSTANCE dereference (the macro yields the address of the tag data
 * pointer). The terminal `(max-1 != level) - 1` is the database's branchless "wrap to -1 at the end". */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_magazine_state.h"
#include "headers/blam_data_globals.h"


int16_t weapon_rotate_zoom_level(int weapon_index, int16_t zoom_level)
{
    weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    int hold_level = 0;
    int16_t max_levels;

    if ( definition->weapon.magazines.count > 0 )
        hold_level = (uint16_t)weapon->weapon.magazines[0].state == _weapon_magazine_reloading;
    if ( hold_level )
        return zoom_level;

    max_levels = definition->weapon.zoom_level_count;
    if ( zoom_level < 0 || zoom_level >= max_levels - 1 )
        return (int16_t)((max_levels - 1 != zoom_level) - 1);   /* wrap to -1 at the last level */
    return (int16_t)(zoom_level + 1);
}
