/* vehicle_scripting_load_magic @0x836D3A34 — script command: instantly seat a list of units into a
 * vehicle. Finds the vehicle's available seats matching seat_substring_name, then for each object in the
 * object list tries each free seat in turn: a seat is taken if the rider is itself a vehicle or its
 * seat/weapon label matches the seat. A rider already in a seat is ejected first; on a successful enter the
 * seat is consumed and the loaded count incremented. Returns the number of units seated.
 *
 * Object datum offsets: type @+180, flags @+262, parent_object_index @+284, parent_seat_index @+752 (u16,
 * 0xFFFF = none). The vehicle definition's seat block is vehicle_def->unit.seats (address at abs +744);
 * each unit_seat is 284 bytes with its label at +4. */

#include <stdint.h>
#include "headers/unit_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/unit_seat.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/object_type.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"


extern int16_t vehicle_scripting_find_available_seats(int unit_index, const char *seat_substring_name, int16_t seat_desire_type, int16_t *seat_indices, int16_t seat_max_indices);
extern int object_list_get_first(int object_list_index, int *reference_index);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern uint8_t unit_set_or_test_seat_and_weapon_label(int object_index, const char *seat_label, const char *weapon_label, uint8_t change_flag);
extern void unit_exit_seat_end(int object_index, uint8_t is_part_of_non_forced_exit, uint8_t should_allow_clients, uint8_t should_replicate_if_appropriate);
extern uint8_t unit_enter_seat(int unit_index, int parent_unit_index, int16_t seat_index);

int16_t vehicle_scripting_load_magic(int unit_index, const char *seat_substring_name, int object_list_index)
{
    int16_t loaded_count = 0;
    if (unit_index == -1)
        return loaded_count;

    unit_datum *vehicle = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, vehicle->definition_index);

    int16_t available_seat_indices[16];
    int16_t available_seats = vehicle_scripting_find_available_seats(unit_index, seat_substring_name, -1,
                                                                     available_seat_indices, 16);

    int object_list_reference[4];
    for (int i = object_list_get_first(object_list_index, object_list_reference); i != -1;
         i = object_list_get_next(object_list_index, object_list_reference))
    {
        unit_datum *rider = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, i)->datum);

        if (((1 << rider->object.type) & object_mask_unit) == 0
            || (vehicle->object.damage_flags & (1u << _object_dead_bit)) != 0
            || available_seats <= 0)
            continue;

        int seat_slot = 0;
        uint8_t entered = 0;
        while (1)
        {
            int16_t seat_index = available_seat_indices[seat_slot];
            const char *seat_label = ((unit_seat *)vehicle_def->unit.seats.address + seat_index)->label;
            if (seat_index != -1
                && (rider->object.type == object_type_vehicle
                    || unit_set_or_test_seat_and_weapon_label(i, seat_label, nullptr, 0)))
            {
                if (rider->object.parent_object_index != -1 && rider->unit.parent_seat_index != 0xFFFF)
                    unit_exit_seat_end(i, 0, 0, 1u);
                if (rider->object.parent_object_index == -1 && unit_enter_seat(i, unit_index, seat_index))
                {
                    entered = 1;
                    break;
                }
            }

            seat_slot = (int16_t)(seat_slot + 1);
            if (seat_slot >= available_seats)
                break;
        }

        if (entered)
        {
            available_seat_indices[seat_slot] = -1;
            loaded_count = (int16_t)(loaded_count + 1);
        }
    }

    return loaded_count;
}
