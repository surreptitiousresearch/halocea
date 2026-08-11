/* unit_flying_through_air @0x836CD858 — true when a unit is airborne. Units carrying an animation
 * graph state (object.type nonzero, i.e. not a biped) are never considered "flying"; otherwise defer to
 * the biped-specific airborne test. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern uint8_t biped_flying_through_air(int biped_index);

uint8_t unit_flying_through_air(int unit_index)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    if ( unit->object.type )
        return 0;
    return biped_flying_through_air(unit_index);
}
