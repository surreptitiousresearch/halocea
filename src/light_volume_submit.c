/* light_volume_submit @0x8380B150 — submit a light-volume widget for rendering on an object, if valid:
 * resolves the widget's definition tag (whose marker name lives inline at its own base address — no
 * dedicated header for this tag yet), skips it unless it has both a positive "count" field (+110) and a
 * positive "size" field (+288), skips it if its driving animation value (+68, a 1-based index into
 * `animation->values`) is present but non-positive, resolves the named marker on the object, and — unless
 * a configured maximum distance (+56) is set and the marker is farther from the camera than that along the
 * camera's forward axis — submits the widget for rendering via light_volume_render at the marker's
 * position. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/light_volume_globals.h"
#include "headers/light_volume_datum.h"
#include "headers/light_volume_definition.h"
#include "headers/render_globals.h"
#include "headers/render_lighting.h"
#include "headers/render_animation.h"
#include "headers/object_marker.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


extern void *datum_get(data_array *data, int index);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void rasterizer_widget_submit(int object_index, int widget_index, const real_point3d *centroid,
        void (*render_proc)(int, int));
extern void light_volume_render(int object_index, int widget_index);

void light_volume_submit(int object_index, int widget_index, const render_lighting *lighting,
    const render_animation *animation)
{
    if ( object_index == -1 || widget_index == -1 )
        return;

    light_volume_datum *light_volume = datum_get(light_volume_globals.light_volume_data, widget_index);
    light_volume_definition *definition = TAG_GET(light_volume_definition, light_volume->definition_index);

    if ( definition->count <= 0 || definition->frames.count <= 0 )
        return;

    __int16 animation_value_index = definition->brightness_scale_source;
    if ( animation_value_index && animation && animation->values[animation_value_index - 1] <= 0.0f )
        return;

    object_marker marker;
    object_get_marker_by_name(object_index, definition->attachment_marker, &marker, 1);

    float max_distance = definition->far_fade_distance;
    if ( max_distance == 0.0f
      || render.camera.forward.n[0] * (marker.matrix.n[3][0] - render.camera.position.n[0])
       + render.camera.forward.n[1] * (marker.matrix.n[3][1] - render.camera.position.n[1])
       + render.camera.forward.n[2] * (marker.matrix.n[3][2] - render.camera.position.n[2]) < max_distance )
    {
        rasterizer_widget_submit(object_index, widget_index, (const real_point3d *)&marker.matrix.n[3], light_volume_render);
    }
}
