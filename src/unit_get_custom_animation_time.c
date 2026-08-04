/* unit_get_custom_animation_time @0x836CCEC8 — return the number of frames remaining in a unit's currently
 * playing custom animation, or 0 if the unit is not playing one (animation-state field +0x2A3 != 28). The
 * total frame count comes from the animation definition (animations block of the unit's animation graph tag,
 * indexed by the current animation index +0xD0); the result is max(0, frame_count - current_frame - 2). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/animation_graph.h"
#include "headers/animation.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_animation_state.h"
#include "headers/blam_data_globals.h"


int16_t unit_get_custom_animation_time(int unit_index)
{
    if ( unit_index == -1 )
        return 0;
    unit_datum *unit =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    if ( unit->unit.animation.state != _unit_state_user_animation )
        return 0;

    animation_graph *graph = TAG_GET(animation_graph, unit->object.animation.animation_graph_index);
    animation *anim = &((animation *)graph->animations.address)[unit->object.animation.state.index];
    __int16 frame_count = anim->frame_count;

    int remaining = frame_count - unit->object.animation.state.frame_index - 2;
    return remaining < 0 ? 0 : remaining;   /* PPC saturate idiom => max(0, remaining) */
}
