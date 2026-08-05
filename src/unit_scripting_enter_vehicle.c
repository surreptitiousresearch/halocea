/* unit_scripting_enter_vehicle @0x836D3878 — make a unit enter a named seat of a parent unit. The unit first
 * exits any current seat; then, for the parent's vehicle definition seat (count +0x2E4, block +0x2E8, stride
 * 284, name at +4) whose name matches, it enters that seat provided the seat is not already filled and the
 * unit either is itself a vehicle (object type 1) or passes the seat/weapon label test. No-op for a dead unit
 * (flags +0x106 bit 0x4) or an empty seat name. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/global_tag_instances.h"
#include "headers/object_type.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"

extern int stricmp(const char *a, const char *b);
extern void unit_exit_seat_end(int object_index, uint8_t is_part_of_non_forced_exit, uint8_t should_allow_clients, uint8_t should_replicate_if_appropriate);
extern uint8_t unit_seat_filled(int parent_object_index, int16_t seat_index);
extern uint8_t unit_set_or_test_seat_and_weapon_label(int object_index, const char *seat_label, const char *weapon_label, uint8_t change_flag);
extern uint8_t unit_enter_seat(int unit_index, int parent_unit_index, int16_t seat_index);

void unit_scripting_enter_vehicle(int unit_index, int parent_unit_index, const char *seat_name)
{
    if ( unit_index == -1 || parent_unit_index == -1 )
        return;

    /* Empty seat name (single terminating NUL) means "no seat". */
    const char *scan = seat_name;
    while ( *scan++ )
        ;
    if ( scan - seat_name == 1 )
        return;

    unit_datum *unit =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    if ( (unit->object.damage_flags & (1u << _object_dead_bit)) != 0 )
        return;

    if ( unit->object.parent_object_index != -1 && (uint16_t)unit->unit.parent_seat_index != 0xFFFF )
        unit_exit_seat_end(unit_index, 0, 0, 1u);

    if ( unit->object.parent_object_index != -1 )
        return;

    unit_datum *parent =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_unit_index)->datum;
    unit_definition *vehicle_definition = TAG_GET(unit_definition, parent->definition_index);
    int seat_count = vehicle_definition->unit.seats.count;
    if ( seat_count <= 0 )
        return;

    for ( int16_t seat = 0; seat < seat_count; seat = (int16_t)(seat + 1) )
    {
        const char *seat_definition_name = ((unit_seat *)vehicle_definition->unit.seats.address)[seat].label;
        if ( !stricmp(seat_name, seat_definition_name)
          && !unit_seat_filled(parent_unit_index, seat)
          && (unit->object.type == object_type_vehicle || unit_set_or_test_seat_and_weapon_label(unit_index, seat_definition_name, nullptr, 0)) )
        {
            unit_enter_seat(unit_index, parent_unit_index, seat);
            return;
        }
    }
}
