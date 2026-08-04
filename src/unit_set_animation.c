/* unit_set_animation @0x836CC0E0 — directly set a unit's animation: store the animation graph index
 * (unit data +0xCC), the animation index (+0xD0) and reset the frame counter (+0xD2) to 0. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


void unit_set_animation(int unit_index, int animation_graph_index, int16_t animation_index)
{
    unit_datum *unit_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit_data->object.animation.animation_graph_index = animation_graph_index;
    unit_data->object.animation.state.index = animation_index;
    unit_data->object.animation.state.frame_index = 0;
}
