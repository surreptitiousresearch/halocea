/* ai_place_pending_mounted_weapons @ 0x836E90A8 — spawn the AI gunners queued for mounted-weapon units
 * this tick. For each queued unit, walk the unit definition's seats; any seat that carries a built-in
 * actor variant gets an actor placed at the unit's origin and seated into that seat.
 * The pending queue is then cleared. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/ai_globals.h"
#include "headers/actor_starting_location_definition.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern int actor_place(int actor_variant_definition_index, int encounter_index, int16_t squad_index, actor_starting_location_definition *starting_location, uint8_t upgrade_major, int16_t variant_number);
extern uint8_t unit_enter_seat(int unit_index, int parent_unit_index, int16_t seat_index);

void ai_place_pending_mounted_weapons(void)
{
    if ( ai_globals->mounted_weapon_unit_count <= 0 )
    {
        ai_globals->mounted_weapon_unit_count = 0;
        return;
    }

    for ( int queue_index = 0; queue_index < ai_globals->mounted_weapon_unit_count; queue_index = (__int16)(queue_index + 1) )
    {
        int object_index = ai_globals->mounted_weapon_unit_indices[queue_index];

        unit_datum *object_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
        unit_definition *definition = TAG_GET(unit_definition, object_data->definition_index);

        if ( definition->unit.seats.count > 0 )
        {
            __int16 seat_index = 0;
            do
            {
                unit_seat *seat = (unit_seat *)definition->unit.seats.address + seat_index;
                if ( seat->built_in_actor_reference.index != -1 )
                {
                    actor_starting_location_definition starting_location;
                    memset(&starting_location, 0, sizeof(starting_location));
                    starting_location.command_list_index = -1;
                    object_get_origin(object_index, &starting_location.position);

                    int actor_index = actor_place(seat->built_in_actor_reference.index, -1, -1, &starting_location, 0, 0);
                    if ( actor_index != -1 )
                        unit_enter_seat(DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index)->meta.unit_index,
                                        object_index, seat_index);
                }
                seat_index = (__int16)(seat_index + 1);
            }
            while ( seat_index < definition->unit.seats.count );
        }
    }
    ai_globals->mounted_weapon_unit_count = 0;
}
