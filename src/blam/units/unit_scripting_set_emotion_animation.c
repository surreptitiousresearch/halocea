/* unit_scripting_set_emotion_animation @0x836C9C98 — set a unit's emotion animation (stored at unit +0x21E)
 * by looking the animation up by name in the unit's animation graph (+0xCC); warns if it is not found. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern int16_t animation_graph_get_animation_by_name(int animation_graph_index, const char *animation_name);
extern void console_warning(const char *format, ...);

void unit_scripting_set_emotion_animation(int unit_index, const char *animation_name)
{
    if ( unit_index == -1 )
        return;
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int16_t animation_index = animation_graph_get_animation_by_name(unit->object.animation.animation_graph_index, animation_name);
    if ( animation_index == -1 )
        console_warning("couldn't find the emotion animation '%s'", animation_name);
    else
        unit->unit.override_emotion_animation_index = animation_index;
}
