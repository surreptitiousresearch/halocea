/* unit_get_animation_frames_remaining @0x836C73F8 — frames left in the unit's current animation: total frame
 * count of the animation (from the animation graph tag) minus the current frame. Also outputs the unit's
 * animation state byte. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/animation_graph.h"
#include "headers/animation.h"
#include "headers/blam_data_globals.h"


int16_t unit_get_animation_frames_remaining(int unit_index, short *animation_state)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    animation_graph *graph = TAG_GET(animation_graph, unit->object.animation.animation_graph_index);
    animation *current = &((animation *)graph->animations.address)[unit->object.animation.state.index];
    *animation_state = unit->unit.animation.state;
    /* extsh at 0x836C7460 is the ABI 16-bit return normalization, implicit under int16_t return */
    return current->frame_count - unit->object.animation.state.frame_index;
}
