/* unit_ready_desired_weapon @ 0x836D23F8 — switch the unit to the weapon it currently wants ready
 * (unit.desired_weapon_index). Any weapon already in the active slot (unit.current_weapon_index) is first put
 * away: detached, disconnected from the map, deactivated, hidden, and returned to inventory. If no desired
 * weapon exists the unit goes "unarmed" (seat/weapon label set, slot cleared) and unzooms. Otherwise the
 * desired weapon's label is applied to the seat, the weapon is reconnected/shown and attached to the unit's
 * hand/grip markers (resolved through the unit definition's animation graph unit_seats block, indexed by the
 * unit's animation seat/weapon indices), the active slot is set, its ready time stamped, and weapon_ready
 * fires. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_unit_seat.h"
#include "headers/animation_graph_weapon_class.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


#include "headers/location.h"
extern uint8_t weapon_put_away(int weapon_index, uint8_t immediate);
extern void object_detach(int child_object_index);
extern void object_disconnect_from_map(int object_index);
extern void object_activate(int object_index);
extern void object_set_visibility(int object_index, uint8_t visible_flag);
extern void item_in_unit_inventory(int item_index, int owner_unit_index);
extern const char *unit_get_seat_label(int object_index);
extern const char *weapon_get_label(int weapon_index);
extern uint8_t unit_set_or_test_seat_and_weapon_label(int object_index, const char *seat_label, const char *weapon_label, uint8_t change_flag);
extern void unit_unzoom(int unit_index);
extern void object_reconnect_to_map(int object_index, const location *location_in);
extern void object_attach_to_marker(int parent_object_index, const char *parent_marker_name, int child_object_index, const char *child_marker_name);
extern int game_time_get(void);
extern void weapon_ready(int weapon_index);

void unit_ready_desired_weapon(int unit_index, uint8_t immediate)
{
    int desired_weapon = -1;
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    int16_t desired_slot = unit->unit.desired_weapon_index;
    unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);
    if ( desired_slot != -1 )
        desired_weapon = unit->unit.weapon_object_indices[desired_slot];

    unit_datum *unit_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int active_slot = unit_data->unit.current_weapon_index;
    if ( active_slot != -1 )
    {
        int active_weapon = unit_data->unit.weapon_object_indices[active_slot];
        if ( active_weapon != -1 )
        {
            if ( weapon_put_away(active_weapon, immediate) )
            {
                object_detach(active_weapon);
                object_disconnect_from_map(active_weapon);
                object_activate(active_weapon);
                object_set_visibility(active_weapon, 0);
                item_in_unit_inventory(active_weapon, unit_index);
                unit->unit.current_weapon_index = -1;
            }
        }
    }

    if ( unit->unit.current_weapon_index == -1 )
    {
        if ( desired_weapon == -1 )
        {
            const char *seat_label = unit_get_seat_label(unit_index);
            unit_set_or_test_seat_and_weapon_label(unit_index, seat_label, "unarmed", 1u);
            unit->unit.current_weapon_index = -1;
            unit_unzoom(unit_index);
            return;
        }

        const char *weapon_label = weapon_get_label(desired_weapon);
        const char *seat_label = unit_get_seat_label(unit_index);
        unit_set_or_test_seat_and_weapon_label(unit_index, seat_label, weapon_label, 1u);

        animation_graph *graph =
            TAG_GET(animation_graph, definition->object.animation_graph.index);
        animation_graph_unit_seat *seat =
            &((animation_graph_unit_seat *)graph->unit_seats.address)[unit->unit.animation.seat_index];
        animation_graph_weapon_class *weapon_class =
            &((animation_graph_weapon_class *)seat->weapon_classes.address)[unit->unit.animation.weapon_index];

        object_reconnect_to_map(desired_weapon, nullptr);
        object_set_visibility(desired_weapon, 1u);
        object_attach_to_marker(unit_index, weapon_class->hand_marker_name, desired_weapon,
            weapon_class->grip_marker_name);

        int16_t new_slot = unit->unit.desired_weapon_index;
        unit->unit.current_weapon_index = new_slot;
        if ( (uint16_t)new_slot != 0xFFFF )
            unit->unit.weapon_last_used_at_game_time[unit->unit.current_weapon_index] = game_time_get();
        weapon_ready(desired_weapon);
    }
    unit_unzoom(unit_index);
}
