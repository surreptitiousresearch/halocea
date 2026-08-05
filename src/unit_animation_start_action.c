/* unit_animation_start_action @0x836C8340 — play a unit "action" animation (a full-body replacement action
 * such as a gesture, throw, or yelo melee, as opposed to an additive overlay). An action of 0 clears the
 * current action. Otherwise the action enum maps either to a weapon-level animation type (resolved against
 * the weapon class's own animation table) or a weapon-type-level animation type (resolved against the
 * weapon-type animation block); whichever matches is started as a random permutation in action_animation,
 * with a blend-in interpolation (0 frames for action 7, else 6).
 *
 * Graph navigation mirrors unit_animation_start_overlay_action: unit definition animation graph ->
 * unit_seats (by animation.seat_index) -> weapon_classes (by animation.weapon_index) -> weapon_types
 * (by animation.weapon_type_index). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_unit_seat.h"
#include "headers/animation_graph_weapon_class.h"
#include "headers/animation_graph_weapon_type.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_animation_action.h"
#include "headers/weapon_class_animation.h"
#include "headers/weapon_type_animation.h"
#include "headers/blam_data_globals.h"
#include "headers/animation_update_kind.h"


extern void object_start_interpolation(int object_index, int16_t frame_count);
extern int16_t animation_choose_random_permutation_internal(int render_or_affects_game_state, int animation_graph_index, int16_t animation_index);

void unit_animation_start_action(int unit_index, int16_t action)
{
    unit_datum *unit_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    if ( !action )
    {
        unit_data->unit.animation.action = 0;
        unit_data->unit.animation.action_animation.index = -1;
        return;
    }

    unit_definition *definition = TAG_GET(unit_definition, unit_data->definition_index);
    int16_t weapon_animation_type = -1;        /* resolved against the weapon class table */
    int16_t weapon_type_animation_type = -1;   /* resolved against the weapon-type block */
    int16_t animation_index = -1;

    animation_graph *graph = TAG_GET(animation_graph, definition->object.animation_graph.index);
    animation_graph_unit_seat *seat_entry =
        &((animation_graph_unit_seat *)graph->unit_seats.address)[unit_data->unit.animation.seat_index];
    animation_graph_weapon_class *weapon_entry =
        &((animation_graph_weapon_class *)seat_entry->weapon_classes.address)[unit_data->unit.animation.weapon_index];
    animation_graph_weapon_type *weapon_type_block =
        &((animation_graph_weapon_type *)weapon_entry->weapon_types.address)[unit_data->unit.animation.weapon_type_index];

    if ( action >= _unit_animation_action_disarm && action <= _unit_animation_action_overheat )
    {
        switch ( action )
        {
            case _unit_animation_action_disarm:        weapon_animation_type = _weapon_class_animation_disarm;        break;
            case _unit_animation_action_drop:          weapon_animation_type = _weapon_class_animation_drop;          break;
            case _unit_animation_action_ready:         weapon_animation_type = _weapon_class_animation_ready;         break;
            case _unit_animation_action_put_away:      weapon_animation_type = _weapon_class_animation_put_away;      break;
            case _unit_animation_action_reload_1:      weapon_type_animation_type = _weapon_type_animation_reload_1;  break;
            case _unit_animation_action_reload_2:      weapon_type_animation_type = _weapon_type_animation_reload_2;  break;
            case _unit_animation_action_melee:         weapon_type_animation_type = _weapon_type_animation_melee;     break;
            case _unit_animation_action_throw_grenade: weapon_animation_type = _weapon_class_animation_throw_grenade; break;
            default: /* _unit_animation_action_overheat */
                weapon_type_animation_type = _weapon_type_animation_overheat;
                break;
        }
    }

    if ( weapon_type_animation_type == -1 )
    {
        if ( weapon_animation_type != -1 )
        {
            if ( weapon_animation_type >= 0 && weapon_animation_type < weapon_entry->animations.count )
                animation_index = ((int16_t *)weapon_entry->animations.address)[weapon_animation_type];
            else
                animation_index = -1;
        }
    }
    else
    {
        if ( weapon_type_animation_type < 0 || weapon_type_animation_type >= weapon_type_block->animations.count )
            animation_index = -1;
        else
            animation_index = ((int16_t *)weapon_type_block->animations.address)[weapon_type_animation_type];
    }

    int16_t interpolation_frames = (action == _unit_animation_action_melee) ? 0 : 6;
    if ( animation_index != -1 )
    {
        if ( interpolation_frames > 0 )
            object_start_interpolation(unit_index, interpolation_frames);
        unit_data->unit.animation.action_animation.index = animation_choose_random_permutation_internal(
            animation_update_kind_affects_game_state, definition->object.animation_graph.index, animation_index);
        unit_data->unit.animation.action_animation.frame_index = 0;
        unit_data->unit.animation.action = (char)action;
    }
}
