/* animation_update_internal @0x837969A0 — advance one animation by a frame and report what happened.
 * Optionally reports the sound a frame triggers (via triggered_sound_index). Advances the frame; if still
 * within the clip it returns a key-frame code (0/1 from the two key frames, or 2 on the last-but-one frame
 * with no loop). At the clip end it either jumps to the loop frame (return 4) or chooses a random next
 * permutation and restarts (return 3).
 *
 * The animation graph's animation records and sound-reference records are the typed animation /
 * animation_graph_sound_reference DB structs. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/animation_graph.h"
#include "headers/animation.h"
#include "headers/animation_graph_sound_reference.h"
#include "headers/animation_state.h"
#include "headers/animation_update_kind.h"
#include "headers/animation_update_result.h"

extern int16_t animation_choose_random_permutation_internal(int render_or_affects_game_state, int animation_graph_index, int16_t animation_index);

/* return attested int16_t: 3/5 callers extsh r3 (first_person_weapon_update, weapon_update, scenery_update) */
int16_t animation_update_internal(animation_update_kind render_or_affects_game_state, int animation_graph_index,
                                  animation_state *state, int *triggered_sound_index)
{
    animation_graph *graph = TAG_GET(animation_graph, animation_graph_index);
    animation *anim = &((animation *)graph->animations.address)[state->index];

    if (triggered_sound_index)
    {
        int sound_event = anim->sound_index;
        if (sound_event == -1 || anim->private_sound_frame_index != state->frame_index)
            *triggered_sound_index = -1;
        else
            *triggered_sound_index = ((animation_graph_sound_reference *)graph->sound_references.address)[sound_event].sound.index;
    }

    int16_t next_frame = (int16_t)(state->frame_index + 1);
    state->frame_index = next_frame;
    int16_t frame_count = anim->frame_count;

    if (next_frame < frame_count)
    {
        if ((int16_t)(next_frame + 1) != frame_count || anim->private_loop_frame_index)
            return (state->frame_index == anim->private_key_frame_index || state->frame_index == anim->private_second_key_frame_index) ? _animation_key_frame : _animation_running;
        else
            return _animation_will_restart_on_next_frame;
    }
    else
    {
        int16_t loop_frame = anim->private_loop_frame_index;
        if (loop_frame <= 0)
        {
            state->index = animation_choose_random_permutation_internal(render_or_affects_game_state,
                                                                        animation_graph_index, anim->runtime_parent_animation_index);
            state->frame_index = 0;
            return _animation_restarted;
        }
        else
        {
            if (loop_frame > frame_count - 1)
                loop_frame = frame_count - 1;
            state->frame_index = loop_frame;
            return _animation_looped;
        }
    }
}
