/* unit_start_user_animation @0x836CCFB0 — begin playing a named "user" (custom) animation on a unit from an
 * animation graph tag. Resolves the animation by name, chooses a random permutation, and validates it can be
 * started (rejects animations flagged at +0x20, and only restarts an already-playing custom animation if the
 * new one is a compatible continuation). On success it interpolates (if requested), records the graph/animation
 * /frame state (unit +0xCC/+0xD0/+0xD2), sets the custom-animation state (+0x2A3 = 28) and a dirty flag
 * (+0x298 bit 0x1), and recomputes node matrices. Returns true on success.
 *
 * DEVIATION: the PPC carry idiom `!__CFADD__(frame - start, 0x80000000)` is a signed test (frame >= start);
 * reconstructed as such. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/animation_graph.h"
#include "headers/animation.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_animation_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/blam_data_globals.h"
#include "headers/animation_update_kind.h"

extern int16_t animation_graph_get_animation_by_name(int animation_graph_index, const char *animation_name);
extern char *tag_get_name(int tag_index);
extern void console_warning(const char *format, ...);
extern int16_t animation_choose_random_permutation_internal(int render_or_affects_game_state, int animation_graph_index, int16_t animation_index);
extern void object_start_interpolation(int object_index, int16_t frame_count);
extern void object_compute_node_matrices_recursive(int object_index);


uint8_t unit_start_user_animation(int unit_index, int animation_graph_index, const char *animation_name, uint8_t interpolate)
{
    if ( unit_index == -1 || animation_graph_index == -1 )
        return 0;

    animation_graph *graph = TAG_GET(animation_graph, animation_graph_index);
    unit_datum *unit =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    int16_t animation_index = animation_graph_get_animation_by_name(animation_graph_index, animation_name);
    if ( animation_index == -1 )
    {
        console_warning("the animation '%s' doesn't exist in the graph '%s'", animation_name, tag_get_name(animation_graph_index));
        return 0;
    }

    int16_t permutation = animation_choose_random_permutation_internal(
        animation_update_kind_affects_game_state, animation_graph_index, animation_index);
    animation *animations_block = (animation *)graph->animations.address;
    animation *anim = &animations_block[permutation];
    if ( anim->type != 0 )
        return 0;

    unsigned char can_start = 1;
    if ( unit->unit.animation.state == _unit_state_user_animation )
    {
        int current_animation_index = unit->object.animation.state.index;
        if ( current_animation_index != -1 )
        {
            animation *current_animation = &animations_block[current_animation_index];
            if ( current_animation->runtime_parent_animation_index == anim->runtime_parent_animation_index )
            {
                int16_t final_frame = current_animation->private_key_frame_index;
                int current_frame = unit->object.animation.state.frame_index;
                if ( current_frame + 2 == final_frame )
                {
                    can_start = 0;
                    unit->object.animation.state.frame_index = current_frame - 1;
                }
                else
                {
                    can_start = (current_frame - final_frame) >= 0;
                }
            }
        }
    }
    if ( !can_start )
        return 0;

    if ( interpolate )
        object_start_interpolation(unit_index, 6);
    unit->unit.animation.state = _unit_state_user_animation;
    unit_datum *unit_reread =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit_reread->object.animation.animation_graph_index = animation_graph_index;
    unit_reread->object.animation.state.index = permutation;
    unit_reread->object.animation.state.frame_index = 0;
    unit->unit.animation.flags |= (1u << _unit_animation_postpone_weapon_ik_until_interpolation_ends_bit);
    object_compute_node_matrices_recursive(unit_index);
    return 1;
}
