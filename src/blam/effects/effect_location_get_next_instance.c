/* effect_location_get_next_instance @0x836E1EC0 — walk an effect's marker-location list, returning the
 * next location appropriate for the requested camera mode and advancing the caller's cursor. First-person
 * locations (node designator bit 0x8000) are only returned for first-person rendering (camera_mode 1, or
 * mode 3 with a single local player); third-person rendering skips them, and vice versa. Returns null at
 * the end of the list. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/effect_datum.h"
#include "headers/effect_location_datum.h"
#include "headers/effect_camera_mode.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern int16_t local_player_count(void);
extern void *datum_get(data_array *data, int index);

effect_location_datum *effect_location_get_next_instance(const effect_datum *effect, int *location_datum_index,
        int16_t camera_mode)
{
    effect_location_datum *location;
    int16_t node_designator;

    if ( *location_datum_index == -1 )
        return NULL;

    /* recovered: effect_location_data->data + 60*(u16)index -> datum_get (60 == effect_location datum_size incl. 2-byte salt header) */
    location = datum_get(effect_location_data, *location_datum_index);
    *location_datum_index = location->next_instance_location_index;   /* advance cursor to next_instance index */

    node_designator = location->node_designator;
    if ( camera_mode == _effect_camera_mode_first_person_only
      || (camera_mode == _effect_camera_mode_both && (uint16_t)effect->local_player_index != 0xFFFF && local_player_count() == 1) )
    {
        /* first-person view: skip non-first-person locations */
        if ( (uint16_t)node_designator == 0xFFFF || (node_designator & 0x8000) == 0 )
            return effect_location_get_next_instance(effect, location_datum_index, camera_mode);
    }
    else
    {
        /* third-person view: skip first-person locations */
        if ( (uint16_t)node_designator != 0xFFFF && (node_designator & 0x8000) != 0 )
            return effect_location_get_next_instance(effect, location_datum_index, camera_mode);
    }
    return location;
}
