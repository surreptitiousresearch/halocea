/* unit_is_busy @0x836C9C68 — true when the unit (a biped/vehicle object) is in a "busy" locomotion state
 * that blocks new actions. Resolves the object's data block, then queries its unit_animation (at +664). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_animation.h"
#include "headers/blam_data_globals.h"

extern uint8_t unit_animation_busy(unit_animation *animation);

uint8_t unit_is_busy(int object_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    /* +664 = unit_datum.unit.animation (500 + 0xA4) */
    return unit_animation_busy(&unit->unit.animation);
}
