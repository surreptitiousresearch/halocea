/* unit_animation_start_overlay_action @0x836C8548 — play a unit overlay animation (an additive action layered
 * over the unit's base animation, e.g. flinch/ping). The overlay only starts if its priority is at least the
 * currently-playing overlay's priority (unit.animation.overlay_action) and the overlay channel is not busy.
 * The overlay_action enum is mapped to an animation type, the type is resolved to a concrete animation index
 * through the unit's animation graph (unit_seats -> weapon_classes -> weapon_types -> animations), and on
 * success the overlay animation is set to a random permutation. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_animation.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_unit_seat.h"
#include "headers/animation_graph_weapon_class.h"
#include "headers/animation_graph_weapon_type.h"
#include "headers/unit_animation_overlay_action.h"
#include "headers/weapon_type_animation.h"
#include "headers/blam_data_globals.h"
#include "headers/animation_update_kind.h"


extern uint8_t unit_animation_busy(unit_animation *animation);
extern int16_t animation_choose_random_permutation_internal(int render_or_affects_game_state, int animation_graph_index, int16_t animation_index);

void unit_animation_start_overlay_action(int unit_index, int16_t overlay_action)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unit_animation *animation = &unit->unit.animation;

    if ( overlay_action >= animation->overlay_action
         && !unit_animation_busy(animation) )
    {
        unsigned int action = overlay_action - 1;
        int animation_graph_index =
            TAG_GET(unit_definition, unit->definition_index)->object.animation_graph.index;
        int16_t animation_type = -1;
        int16_t animation_index = -1;

        animation_graph *graph = TAG_GET(animation_graph, animation_graph_index);
        animation_graph_unit_seat *seat =
            &((animation_graph_unit_seat *)graph->unit_seats.address)[animation->seat_index];
        animation_graph_weapon_class *weapon_class =
            &((animation_graph_weapon_class *)seat->weapon_classes.address)[animation->weapon_index];
        animation_graph_weapon_type *weapon_type =
            &((animation_graph_weapon_type *)weapon_class->weapon_types.address)[animation->weapon_type_index];

        if ( action <= 5 )
        {
            /* `action` is overlay_action - 1; case labels kept in that biased form */
            switch ( action )
            {
                case _unit_animation_overlay_action_fire_2 - 1:    animation_type = _weapon_type_animation_fire_2;    break;
                case _unit_animation_overlay_action_charged_1 - 1: animation_type = _weapon_type_animation_charged_1; break;
                case _unit_animation_overlay_action_charged_2 - 1: animation_type = _weapon_type_animation_charged_2; break;
                case _unit_animation_overlay_action_chamber_1 - 1: animation_type = _weapon_type_animation_chamber_1; break;
                default: /* chamber_2 - 1 (5) or fire_1 - 1 (0) */
                    animation_type = action ? _weapon_type_animation_chamber_2 : _weapon_type_animation_fire_1;
                    break;
            }
        }

        if ( animation_type != -1 )
        {
            if ( animation_type < 0 || animation_type >= weapon_type->animations.count )
                animation_index = -1;
            else
                animation_index = ((int16_t *)weapon_type->animations.address)[animation_type];
        }

        if ( animation_index != -1 )
        {
            animation->overlay_action_animation.index = animation_choose_random_permutation_internal(
                animation_update_kind_affects_game_state, animation_graph_index, animation_index);
            animation->overlay_action_animation.frame_index = 0;
            animation->overlay_action = (char)overlay_action;
        }
    }
}
