/* unit_inventory_next_grenade @ 0x836C9AE8 — pick the next grenade type the unit actually carries,
 * cycling from current_index in the direction of `delta` (forward when >= 0, backward otherwise) across
 * the two grenade types. A type is available when its count (unit.grenade_counts[type]) is positive.
 * Returns the first available type found; if delta is 0 it just returns current_index's type when
 * available. Returns -1 if the unit carries no grenades. current_index == -1 is treated as starting
 * from type 0. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_grenade_type.h"
#include "headers/blam_data_globals.h"


/* r4 read via extsh (int16_t current_index); r5 read via extsh (int16_t delta);
 * return consumed via extsh r3 / sth r3 (int16_t). */
int16_t unit_inventory_next_grenade(int unit_index, int16_t current_index, int16_t delta)
{
    int16_t start_type = (current_index == -1) ? 0 : current_index;
    int type = start_type;
    int result = -1;
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    do
    {
        int16_t current_type = (int16_t)type;
        if ( unit->unit.grenade_counts[(int16_t)type] > 0 )
        {
            result = type;
            if ( (int16_t)type != start_type || !delta )
                break;
        }

        if ( delta >= 0 )
            type = (current_type == _unit_grenade_covenant_plasma) ? _unit_grenade_human_fragmentation : current_type + 1;
        else
            type = (current_type == _unit_grenade_human_fragmentation) ? _unit_grenade_covenant_plasma : current_type - 1;
    }
    while ( (int16_t)type != start_type );

    return result;
}
