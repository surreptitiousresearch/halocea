/* scenery_get_animation_time @0x837EA008 — ticks remaining (minus a 2-frame lead) on a scenery object's
 * current animation, or 0 if it isn't animating (scenery flag bit 0 clear). The current animation's frame
 * count is read from the model-animation tag (object animation graph index, animations block, animation
 * element frame_count); the current frame is the object's animation frame index.
 *
 * DEVIATION: the decompiler's sign-bit/equality bit-twiddle reduces to max(frames_left - 2, 0). */

#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/scenery_datum.h"
#include "headers/animation_graph.h"
#include "headers/animation.h"
#include "headers/scenery_flags.h"
#include "headers/blam_data_globals.h"


int scenery_get_animation_time(int scenery_index)
{
    scenery_datum *scenery = (scenery_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, scenery_index)->datum;
    if ( (scenery->scenery.flags & (1u << _scenery_self_animated_bit)) == 0 )
        return 0;

    animation_graph *definition = TAG_GET(animation_graph, scenery->object.animation.animation_graph_index);
    short current_animation = scenery->object.animation.state.index;
    short total_frames = ((animation *)definition->animations.address)[current_animation].frame_count;
    short current_frame = scenery->object.animation.state.frame_index;

    int frames_left = total_frames - current_frame;
    return frames_left > 2 ? frames_left - 2 : 0;
}
