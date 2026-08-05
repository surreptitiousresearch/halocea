/* unit_custom_animation_at_frame @0x836D0E10 — start a custom animation (via unit_start_user_animation) and
 * then jump it to a specific frame, provided the frame index is within the animation's frame count (animation
 * definition +0x22 in the unit's animation graph). Returns true on success. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/animation.h"
#include "headers/animation_graph.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

extern uint8_t unit_start_user_animation(int unit_index, int animation_graph_index, const char *animation_name, uint8_t interpolate);

uint8_t unit_custom_animation_at_frame(int unit_index, int animation_graph_index, const char *animation_name, uint8_t interpolate, int16_t frame_index)
{
    if ( !unit_start_user_animation(unit_index, animation_graph_index, animation_name, interpolate) )
        return 0;

    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    animation_graph *graph = TAG_GET(animation_graph, unit->object.animation.animation_graph_index);
    animation *animation_record = &((animation *)graph->animations.address)[unit->object.animation.state.index];
    int16_t frame_count = animation_record->frame_count;
    if ( frame_index < 0 || frame_index >= frame_count )
        return 0;

    unit->object.animation.state.frame_index = frame_index;
    return 1;
}
