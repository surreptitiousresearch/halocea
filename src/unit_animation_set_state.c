/* unit_animation_set_state @ 0x836CE230 — request that a unit play the base animation for a high-level
 * animation state (idle/move/turn/throw/etc.). It maps new_state to either a "base" animation index
 * (looked up in the seat's weapon-class animations block) or an "overlay" index (looked up in the seat's
 * own animations block). The chosen permutation is stored in object.animation and an interpolation is
 * started (6 frames default, 1 frame for the idle/move couplings, 2 for the two turn states). It also keeps
 * the aiming-screen and (on first state assignment) the looking permutations in sync. Returns 1 if the
 * state was applied, 0 if it was rejected (no animation for a state that requires one, e.g. grenade/melee
 * states 30-33/39/41). No-ops to a return of 1 when the unit is already in new_state. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_unit_seat.h"
#include "headers/animation_graph_weapon_class.h"
#include "headers/unit_animation_state.h"
#include "headers/weapon_class_animation.h"
#include "headers/unit_seat_animation.h"
#include "headers/blam_data_globals.h"

extern int animation_update_kind_affects_game_state;

extern void unit_throw_grenade_release(int unit_index, uint8_t premature);
extern int16_t animation_choose_random_permutation_internal(int render_or_affects_game_state, int animation_graph_index, int16_t animation_index);
extern int16_t unit_animation_state_get_aiming_screen_index(int16_t state);
extern void object_start_interpolation(int object_index, int16_t frame_count);

uint8_t unit_animation_set_state(int unit_index, int16_t new_state)
{
    char started_new_animation = 0;
    int result = 1;
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    int current_state = unit->unit.animation.state;
    unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);
    int animation_graph_index = definition->object.animation_graph.index;
    int had_no_state = current_state == -1;
    const animation_graph_unit_seat *units_block =
        (const animation_graph_unit_seat *)(TAG_GET(animation_graph, animation_graph_index))->unit_seats.address
        + unit->unit.animation.seat_index;
    const animation_graph_weapon_class *weapon_block =
        (const animation_graph_weapon_class *)units_block->weapon_classes.address + unit->unit.animation.weapon_index;

    __int16 interpolation_frames = 6;

    if ( !(current_state != -1 && new_state == current_state) )
    {
        __int16 base_index = -1;
        __int16 overlay_index = -1;

        if ( current_state == _unit_state_throw_grenade )
            unit_throw_grenade_release(unit_index, 1u);

        switch ( new_state )
        {
            case _unit_state_idle:  base_index = _weapon_class_animation_idle;  break;
            case _unit_state_gesture:  base_index = _weapon_class_animation_gesture;  break;
            case _unit_state_turn_left:  base_index = _weapon_class_animation_turn_left;  break;
            case _unit_state_turn_right:  base_index = _weapon_class_animation_turn_right;  break;
            case _unit_state_move_front:  base_index = _weapon_class_animation_move_front;  break;
            case _unit_state_move_back:  base_index = _weapon_class_animation_move_back;  break;
            case _unit_state_move_left:  base_index = _weapon_class_animation_move_left; break;
            case _unit_state_move_right:  base_index = _weapon_class_animation_move_right; break;
            case _unit_state_stunned_move_front:  base_index = _weapon_class_animation_stunned_front; break;
            case _unit_state_stunned_move_back:  base_index = _weapon_class_animation_stunned_back; break;
            case _unit_state_stunned_move_left: base_index = _weapon_class_animation_stunned_left; break;
            case _unit_state_stunned_move_right: base_index = _weapon_class_animation_stunned_right; break;
            case _unit_state_slide_front: base_index = _weapon_class_animation_slide_front; break;
            case _unit_state_slide_back: base_index = _weapon_class_animation_slide_back; break;
            case _unit_state_slide_left: base_index = _weapon_class_animation_slide_left; break;
            case _unit_state_slide_right: base_index = _weapon_class_animation_slide_right; break;
            case _unit_state_flying_front: overlay_index = _unit_seat_animation_flying_front; break;
            case _unit_state_flying_back: overlay_index = _unit_seat_animation_flying_back; break;
            case _unit_state_flying_left: overlay_index = _unit_seat_animation_flying_left; break;
            case _unit_state_flying_right: overlay_index = _unit_seat_animation_flying_right; break;
            case _unit_state_airborne: base_index = _weapon_class_animation_airborne; break;
            case _unit_state_land_soft: base_index = _weapon_class_animation_land_soft; break;
            case _unit_state_land_hard: base_index = _weapon_class_animation_land_hard; break;
            case _unit_state_dying_airborne: overlay_index = _unit_seat_animation_airborne_dead; break;
            case _unit_state_dying: overlay_index = _unit_seat_animation_landing_dead; break;
            case _unit_state_melee_attack: base_index = _weapon_class_animation_melee; break;
            case _unit_state_melee_airborne: base_index = _weapon_class_animation_melee_airborne; break;
            case _unit_state_melee_continuous: base_index = _weapon_class_animation_melee_continuous; break;
            case _unit_state_throw_grenade: base_index = _weapon_class_animation_throw_grenade; break;
            case _unit_state_resurrect_front: base_index = _weapon_class_animation_resurrect_front; break;
            case _unit_state_resurrect_back: base_index = _weapon_class_animation_resurrect_back; break;
            case _unit_state_feeding: base_index = _weapon_class_animation_feeding; break;
            case _unit_state_opening: overlay_index = _unit_seat_animation_opening; break;
            case _unit_state_closing: overlay_index = _unit_seat_animation_closing; break;
            case _unit_state_leap_start: base_index = _weapon_class_animation_leap_start; break;
            case _unit_state_leap_airborne: base_index = _weapon_class_animation_leap_airborne; break;
            case _unit_state_leap_melee: base_index = _weapon_class_animation_leap_melee; break;
            case _unit_state_hovering: overlay_index = _unit_seat_animation_hovering; break;
            default: break;
        }

        __int16 animation_index;
        if ( base_index == -1 )
        {
            if ( overlay_index == -1 )
                animation_index = -1;
            else if ( overlay_index >= 0 && overlay_index < units_block->animations.count )
                animation_index = ((const __int16 *)units_block->animations.address)[overlay_index];
            else
                animation_index = -1;
        }
        else
        {
            if ( base_index < 0 || base_index >= weapon_block->animations.count )
                animation_index = -1;
            else
                animation_index = ((const __int16 *)weapon_block->animations.address)[base_index];
        }

        int proceed = 1;
        if ( animation_index == -1 )
        {
            char requires_animation = 1;
            switch ( new_state )
            {
                case _unit_state_melee_attack: case _unit_state_melee_airborne: case _unit_state_melee_continuous: case _unit_state_throw_grenade: case _unit_state_leap_start: case _unit_state_leap_melee:
                    requires_animation = 0;
                    break;
                default:
                    break;
            }
            if ( !requires_animation )
            {
                result = 0;
                proceed = 0;
            }
        }

        if ( proceed )
        {
            __int16 permutation = animation_choose_random_permutation_internal(
                animation_update_kind_affects_game_state, animation_graph_index, animation_index);
            unit_datum *unit_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
            unit_data->object.animation.animation_graph_index = animation_graph_index;
            unit_data->object.animation.state.index = permutation;
            unit_data->object.animation.state.frame_index = 0;
            interpolation_frames = 6;
            if ( (new_state == _unit_state_idle || new_state == _unit_state_turn_left || new_state == _unit_state_turn_right)
                && (current_state == _unit_state_idle || current_state == _unit_state_turn_left || current_state == _unit_state_turn_right) )
                interpolation_frames = 1;
            if ( new_state == _unit_state_land_hard || new_state == _unit_state_land_soft )
                interpolation_frames = 2;
            started_new_animation = 1;
        }
    }

    if ( result )
    {
        __int16 aiming_screen_index = unit_animation_state_get_aiming_screen_index(new_state);
        if ( had_no_state
            || aiming_screen_index != unit_animation_state_get_aiming_screen_index(unit->unit.animation.state) )
        {
            __int16 aiming_animation;
            if ( aiming_screen_index < 0 || aiming_screen_index >= weapon_block->animations.count )
                aiming_animation = -1;
            else
                aiming_animation = ((const __int16 *)weapon_block->animations.address)[aiming_screen_index];
            unit->unit.animation.aiming_screen_index = animation_choose_random_permutation_internal(
                animation_update_kind_affects_game_state, animation_graph_index, aiming_animation);
            interpolation_frames = 6;
            started_new_animation = 1;
        }

        if ( had_no_state )
        {
            __int16 looking_animation;
            if ( units_block->animations.count <= _unit_seat_animation_looking )
                looking_animation = -1;
            else
                looking_animation = ((const __int16 *)units_block->animations.address)[_unit_seat_animation_looking];
            unit->unit.animation.looking_screen_index = animation_choose_random_permutation_internal(
                animation_update_kind_affects_game_state, animation_graph_index, looking_animation);
        }

        if ( started_new_animation )
            object_start_interpolation(unit_index, interpolation_frames);
        unit->unit.animation.state = new_state;
    }

    return result;
}
