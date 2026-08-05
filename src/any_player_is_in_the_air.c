/* any_player_is_in_the_air @0x836A84A0 — returns true if any player's controlled unit is currently
 * airborne. A unit counts as airborne if: its ultimate parent object carries the
 * _object_outside_of_map_bit object flag (0x200000, bit 21); or, on foot, a biped that
 * biped_flying_through_air reports as flying; or, in/at a
 * vehicle whose airborne-tick counter (vehicle.airborne_ticks) exceeds 2 — for a vehicle the unit is
 * riding, the vehicle definition's unit-block flags must also carry 0x40. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/unit_definition_flags.h"
#include "headers/object_header_datum.h"
#include "headers/data_iterator.h"
#include "headers/global_tag_instances.h"
#include "headers/player_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/object_type.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


extern int object_get_ultimate_parent(int object_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t biped_flying_through_air(int biped_index);

static vehicle_datum *object_datum_get(int object_index)
{
    return ((vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
}

uint8_t any_player_is_in_the_air(void)
{
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    for (player_datum *player = data_iterator_next(&iterator); player;
         player = data_iterator_next(&iterator))
    {
        int unit_index = player->unit_index;
        if (unit_index == -1)
            continue;

        vehicle_datum *unit = object_datum_get(unit_index);
        int ultimate_parent = object_get_ultimate_parent(unit_index);
        if ((object_datum_get(ultimate_parent)->object.flags & (1u << _object_outside_of_map_bit)) != 0)
            return 1;

        unsigned int airborne_ticks;
        int parent_index = unit->object.parent_object_index;
        if (parent_index == -1)
        {
            if (!(uint16_t)unit->object.type)   /* on-foot biped */
            {
                if (biped_flying_through_air(unit_index))
                    return 1;
                continue;
            }
            if ((uint16_t)unit->object.type != object_type_vehicle)   /* not a vehicle */
                continue;
            airborne_ticks = unit->vehicle.airborne_ticks;
        }
        else
        {
            vehicle_datum *vehicle = object_try_and_get_and_verify_type(parent_index, object_mask_vehicle);
            if (!vehicle
                || (TAG_GET(vehicle_definition, vehicle->definition_index)->unit.flags & (1u << _unit_definition_causes_passenger_dialogue_bit)) == 0)
            {
                continue;
            }
            airborne_ticks = vehicle->vehicle.airborne_ticks;
        }

        if (airborne_ticks > 2)
            return 1;
    }
    return 0;
}
