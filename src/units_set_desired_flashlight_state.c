/* units_set_desired_flashlight_state @0x836CF3D0 — for every biped (object type 3) in an object list,
 * set the desired-flashlight-on (0x10000000) or desired-flashlight-off (0x20000000) request bit in the
 * unit's flags dword (object datum +516). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_type.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"

extern int object_list_get_first(int object_list_index, int *reference_index);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void units_set_desired_flashlight_state(int object_list_index, uint8_t state)
{
    int iterator[12];
    for ( int object_index = object_list_get_first(object_list_index, iterator);
          object_index != -1;
          object_index = object_list_get_next(object_list_index, iterator) )
    {
        if ( object_try_and_get_and_verify_type(object_index, object_mask_unit) )
        {
            unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
            int flags = unit->unit.flags;
            int new_flags = flags | (1u << _unit_desired_integrated_light_on_bit);
            if ( !state )
                new_flags = flags | (1u << _unit_desired_integrated_light_off_bit);
            unit->unit.flags = new_flags;
        }
    }
}
