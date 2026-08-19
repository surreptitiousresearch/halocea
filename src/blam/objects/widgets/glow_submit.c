/* glow_submit @0x8380D240 — updates and renders one glow effect widget attached to an object: resolves the
 * glow's tag name via its definition's tag index, re-resolves the glow's marker (by that name) against the
 * current object, then updates and renders the glow. No-op if either index is -1 (none). */

#include <stdint.h>
#include "headers/glow_globals.h"
#include "headers/glow_datum.h"
#include "headers/render_lighting.h"
#include "headers/render_animation.h"
#include "headers/global_tag_instances.h"

extern void *datum_get(data_array *data, int index);
extern void glow_update(glow_datum *glow, int object_index);
extern void glow_render(int object_index, int widget_index);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);

void glow_submit(int object_index, int widget_index, const render_lighting *lighting, const render_animation *animation)
{
    if ( object_index != -1 && widget_index != -1 )
    {
        glow_datum *glow = datum_get(glow_globals.glow_data, widget_index);
        const char *tag_name = TAG_GET(const char, glow->definition_index);

        glow_update(glow, object_index);

        object_marker marker;
        object_get_marker_by_name(object_index, tag_name, &marker, 1);
        glow_render(object_index, widget_index);
    }
}
