/* unit_enter_seat @0x836D29D8 — seat a unit into a seat of a parent unit (vehicle). Validates the seat,
 * fires the hcex "player_enter_warthog"/"player_enter_banshee" events when a player drives a matching
 * vehicle, attaches the unit to the seat marker (recording the residual offset between the unit origin and
 * the marker so interpolation can ease it into place), reconciles seat/weapon labels, and — if the seat
 * has an "enter" animation — kicks off the entry animation and interpolation. Returns 1 on success, 0 if
 * the seat cannot be entered.
 *
 * DEVIATIONS:
 *   - The two inlined character-compare loops against "vehicles\\warthog\\warthog" /
 *     "vehicles\\banshee\\banshee" are strcmp; restored as strcmp() calls.
 *   - object_attach_to_marker's 4th argument is `&empty_string` (the empty-string marker sentinel — an
 *     attach at the marker origin); the decompiler dropped the address-of.
 *   - player_index_from_unit_index is idempotent here; the decompiler called it twice — called once. */

#include <stdint.h>
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/object_marker.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_seat_animation.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_unit_seat.h"
#include "headers/global_tag_instances.h"
#include "headers/player_datum.h"
#include "headers/unit_animation_state.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"
#include "headers/real_matrix4x3.h"
extern int strcmp(const char *string1, const char *string2);

extern char empty_string;
extern int animation_update_kind_affects_game_state;

extern uint8_t unit_can_enter_seat(int unit_index, int parent_unit_index, int16_t seat_index, int *occupant_unit_index_reference);
extern int player_index_from_unit_index(int unit_index);
extern void hcex_fire_plr_event(const char *eventName, int plr_idx);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern real_vector3d *matrix4x3_inverse_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);
extern void object_attach_to_marker(int parent_object_index, const char *parent_marker_name, int child_object_index, const char *child_marker_name);
extern void unit_update_driver_and_gunner(int parent_object_index);
extern void unit_adjust_for_seat_change(int unit_index);
extern const char *weapon_get_label(int weapon_index);
extern uint8_t unit_set_or_test_seat_and_weapon_label(int object_index, const char *seat_label, const char *weapon_label, uint8_t change_flag);
extern void object_start_interpolation(int object_index, int16_t frame_count);
extern int16_t animation_choose_random_permutation_internal(int render_or_affects_game_state, int animation_graph_index, int16_t animation_index);
extern void object_offset_interpolation(int object_index, const real_vector3d *offset);
extern void object_compute_node_matrices_recursive(int object_index);
extern void ai_handle_enter_vehicle(int unit_index, int vehicle_index);
extern void unit_unzoom(int unit_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

uint8_t unit_enter_seat(int unit_index, int parent_unit_index, int16_t seat_index)
{
    if (!unit_can_enter_seat(unit_index, parent_unit_index, seat_index, nullptr))
        return 0;

    unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    unit_datum *parent_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_unit_index)->datum;
    unit_definition *parent_definition = TAG_GET(unit_definition, parent_object->definition_index);
    unit_seat *seat_definition = &((unit_seat *)parent_definition->unit.seats.address)[seat_index];

    /* fire the hcex player-entered-vehicle event when this unit is player-controlled */
    if (unit_object->unit.player_index != -1)
    {
        const char *vehicle_name = parent_definition->object.model.name;

        int player_index = player_index_from_unit_index(unit_index);
        int16_t player_event_id = player_index == -1
            ? -1
            : DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->local_player_index;

        if (vehicle_name)
        {
            if (!strcmp(vehicle_name, "vehicles\\warthog\\warthog"))
                hcex_fire_plr_event("player_enter_warthog", player_event_id);
            else if (!strcmp(vehicle_name, "vehicles\\banshee\\banshee"))
                hcex_fire_plr_event("player_enter_banshee", player_event_id);
        }
    }

    /* attach to the seat marker, recording the residual marker-local offset for interpolation */
    real_point3d unit_origin;
    object_get_origin(unit_index, &unit_origin);

    object_marker seat_marker;
    object_get_marker_by_name(parent_unit_index, seat_definition->marker_name, &seat_marker, 1);

    real_vector3d seat_offset;
    seat_offset.n[0] = unit_origin.n[0] - seat_marker.matrix.n[3][0];
    seat_offset.n[1] = unit_origin.n[1] - seat_marker.matrix.n[3][1];
    seat_offset.n[2] = unit_origin.n[2] - seat_marker.matrix.n[3][2];
    matrix4x3_inverse_transform_vector(&seat_marker.matrix, &seat_offset, &seat_offset);

    object_attach_to_marker(parent_unit_index, seat_definition->marker_name, unit_index,
                            &empty_string);
    unit_object->unit.parent_seat_index = seat_index;
    unit_object->object.parent_object_index = parent_unit_index;
    unit_update_driver_and_gunner(parent_unit_index);
    unit_adjust_for_seat_change(unit_index);

    /* reconcile the seat's weapon label with the unit's current weapon (or "unarmed") */
    unit_datum *unit_object_reloaded = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int16_t current_weapon_slot = unit_object_reloaded->unit.current_weapon_index;
    int weapon_index;
    const char *weapon_label;
    if (current_weapon_slot == -1
        || (weapon_index = unit_object_reloaded->unit.weapon_object_indices[current_weapon_slot], weapon_index == -1))
        weapon_label = "unarmed";
    else
        weapon_label = weapon_get_label(weapon_index);

    if (!unit_set_or_test_seat_and_weapon_label(unit_index, seat_definition->label, weapon_label, 1))
        unit_set_or_test_seat_and_weapon_label(unit_index, seat_definition->label, nullptr, 1);

    /* play the seat-entry animation, if the unit's animation graph defines one */
    unit_definition *definition = TAG_GET(unit_definition, unit_object->definition_index);
    int animation_graph_index = definition->object.animation_graph.index;
    /* the folded 100 was sizeof(animation_graph_unit_seat) */
    animation_graph_unit_seat *unit_seats =
        (animation_graph_unit_seat *)(TAG_GET(animation_graph, animation_graph_index))->unit_seats.address;
    animation_graph_unit_seat *seat_animations =
        &unit_seats[unit_object->unit.animation.seat_index];

    int16_t enter_animation;
    if (seat_animations->animations.count <= _unit_seat_animation_seat_enter)
        enter_animation = -1;
    else
        enter_animation = ((int16_t *)seat_animations->animations.address)[_unit_seat_animation_seat_enter];

    if (enter_animation != -1)
    {
        object_start_interpolation(unit_index, 6);
        int16_t permutation = animation_choose_random_permutation_internal(
            animation_update_kind_affects_game_state, animation_graph_index, enter_animation);

        unit_datum *unit_object_anim = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
        unit_object_anim->object.animation.animation_graph_index = animation_graph_index;
        unit_object_anim->object.animation.state.index = permutation;
        unit_object_anim->object.animation.state.frame_index = 0;
        unit_object->unit.animation.state = _unit_state_entering_seat;
        object_offset_interpolation(unit_index, &seat_offset);
        object_compute_node_matrices_recursive(unit_index);
    }

    ai_handle_enter_vehicle(unit_index, parent_unit_index);
    unit_unzoom(unit_index);

    vehicle_datum *parent_vehicle = object_try_and_get_and_verify_type(parent_unit_index, object_mask_vehicle);
    if (parent_vehicle)
        parent_vehicle->vehicle.last_controlled_time = -1;

    return 1;
}
