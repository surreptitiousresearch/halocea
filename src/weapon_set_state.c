/* weapon_set_state @0x836DA180 — drive a weapon object into a new animation state. Unless `immediate` is
 * set, the change is gated by an animation-priority test against weapon.state: a higher- or
 * equal-priority new state proceeds, a lower-priority one is rejected (return 0). When the change proceeds
 * it selects the matching animation permutation from the weapon's animation graph (weapon_animations[0])
 * and stores it in animation.state.animation_index, records the new state, resets the frame counter, and
 * notifies the owning unit of the state change.
 *
 * The new_state -> animation-type mapping is the weapon state machine's fixed table (see switch below). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/_object_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_weapon_animations.h"
#include "headers/weapon_datum_state.h"
#include "headers/weapon_animation.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"
#include "headers/animation_update_kind.h"


extern int16_t animation_choose_random_permutation_internal(int render_or_affects_game_state, int animation_graph_index, int16_t animation_index);
extern int weapon_get_owner_object_index(int weapon_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void unit_handle_weapon_state_change(int unit_index, int16_t new_weapon_state);

uint8_t weapon_set_state(int weapon_index, int16_t new_state, uint8_t immediate)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;

    if ( !immediate )
    {
        unsigned char proceed = 0;
        unsigned int current_state = (unsigned char)weapon->weapon.state;
        if ( current_state )
        {
            /* priority test: signed comparison of new_state vs current_state expressed as
             * (new_state>>31) + (new_state>=current_state) + (current_state>>31); non-zero => proceed */
            if ( current_state < 3 )
                proceed = (unsigned char)((new_state >> 31) + (new_state >= (int)current_state)
                                          + ((int)current_state >> 31));
        }
        else
        {
            proceed = 1;   /* no current state: always proceed */
        }

        if ( !proceed )
            return 0;
    }

    int animation_graph_index =
        TAG_GET(_object_definition, weapon->definition_index)->animation_graph.index;
    if ( animation_graph_index != -1 )
    {
        animation_graph *graph = TAG_GET(animation_graph, animation_graph_index);
        if ( graph->weapon_animations.count )
        {
            animation_graph_weapon_animations *animation_mode =
                (animation_graph_weapon_animations *)graph->weapon_animations.address;
            if ( animation_mode )
            {
                int16_t animation_type = -1;
                switch ( new_state )
                {
                    case weapon_state_idle:              animation_type = _weapon_animation_idle;      break;
                    case weapon_state_primary_recoil:    animation_type = _weapon_animation_primary_recoil;    break;
                    case weapon_state_secondary_recoil:  animation_type = _weapon_animation_secondary_recoil;    break;
                    case weapon_state_primary_chamber:   animation_type = _weapon_animation_primary_chamber; break;
                    case weapon_state_secondary_chamber: animation_type = _weapon_animation_secondary_chamber; break;
                    case weapon_state_primary_reload:
                    case weapon_state_secondary_reload:  animation_type = _weapon_animation_primary_reload;  break;
                    case weapon_state_primary_charged:
                    case weapon_state_secondary_charged: animation_type = _weapon_animation_secondary_charged; break;
                    case weapon_state_ready:             animation_type = _weapon_animation_ready;     break;
                    case weapon_state_put_away:          animation_type = _weapon_animation_put_away;  break;
                    default: break;
                }

                if ( animation_type != -1 )
                {
                    int16_t animation_index;
                    if ( animation_type < 0 || animation_type >= animation_mode->animations.count )
                        animation_index = -1;
                    else
                        animation_index = ((int16_t *)animation_mode->animations.address)[animation_type];

                    if ( animation_index != -1 || !new_state )
                    {
                        weapon->object.animation.state.index =
                            animation_choose_random_permutation_internal(
                                animation_update_kind_affects_game_state, animation_graph_index,
                                animation_index);
                        weapon->weapon.state = (unsigned char)new_state;
                        weapon->object.animation.state.frame_index = 0;
                    }
                }
            }
        }
    }

    int owner_object_index = weapon_get_owner_object_index(weapon_index);
    if ( object_try_and_get_and_verify_type(owner_object_index, object_mask_unit) )
        unit_handle_weapon_state_change(owner_object_index, new_state);
    return 1;
}
