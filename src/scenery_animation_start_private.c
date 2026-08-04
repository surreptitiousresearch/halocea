/* scenery_animation_start_private @0x837E9ED8 — start a scenery object playing an animation by name,
 * optionally seeked to a starting frame.
 *
 * The animation is looked up by name in the given animation graph. If it doesn't exist a console warning
 * is printed. Otherwise the scenery object's animation fields are set: the animation index, the "animating"
 * flag (scenery.flags bit 0), the cleared interpolation flag (object.flags bit 7), the graph index, and the
 * starting frame (clamped to the animation's frame count, or 0 when no frame was requested). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/scenery_datum.h"
#include "headers/animation_graph.h"
#include "headers/animation.h"
#include "headers/object_flags.h"
#include "headers/scenery_flags.h"
#include "headers/blam_data_globals.h"

extern int16_t animation_graph_get_animation_by_name(int animation_graph_index, const char *animation_name);
extern char *tag_get_name(int16_t tag_index);
extern void console_warning(const char *format, ...);

void scenery_animation_start_private(int scenery_index, int animation_graph_index, const char *animation_name, int16_t frame_index)
{
    if ( scenery_index == -1 || animation_graph_index == -1 )
        return;

    animation_graph *graph_definition = TAG_GET(animation_graph, animation_graph_index);
    scenery_datum *scenery = (scenery_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, scenery_index)->datum;

    __int16 animation_index = animation_graph_get_animation_by_name(animation_graph_index, animation_name);
    if ( animation_index == -1 )
    {
        console_warning("the animation '%s' doesn't exist in the graph '%s'",
                        animation_name, tag_get_name(animation_graph_index));
        return;
    }

    animation *graph_animations = (animation *)graph_definition->animations.address;

    scenery->object.animation.state.index = animation_index;
    scenery->scenery.flags |= (1u << _scenery_self_animated_bit);   /* animating */
    scenery->object.flags &= ~(1u << _object_animates_automatically_bit);        /* clear interpolation flag (bit 7) */

    if ( frame_index >= 0 )
    {
        __int16 frame_count = graph_animations[animation_index].frame_count;
        __int16 clamped_frame = frame_index;
        if ( frame_index > frame_count - 1 )
            clamped_frame = frame_count - 1;
        scenery->object.animation.animation_graph_index = animation_graph_index;
        scenery->object.animation.state.frame_index = clamped_frame;
    }
    else
    {
        scenery->object.animation.animation_graph_index = animation_graph_index;
        scenery->object.animation.state.frame_index = 0;
    }
}
