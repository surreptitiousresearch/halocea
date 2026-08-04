/* hs_effect_new_from_object_marker @0x837F85D8 — spawn an effect attached to a named marker on an object. The
 * marker transform is resolved in object space and the effect is created attached to the marker's node.
 *
 * DEVIATION: the decompiler invents many spurious trailing parameters; the real signature is
 * (definition_index, object_index, marker_name). The color / impulse-vector-field arguments are left
 * uninitialized by the binary and passed as null. */

#include <stdint.h>
#include "headers/object_marker.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"

#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern int effect_new_attached_from_markers(int definition_index, int owner_object_index, int object_index, int16_t node_index, int16_t marker_count, const char **marker_names, const real_point3d *marker_points, const real_vector3d *marker_forwards, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);

void hs_effect_new_from_object_marker(int definition_index, int object_index, const char *marker_name)
{
    if ( definition_index == -1 || object_index == -1 )
        return;

    object_marker marker;
    if ( object_get_marker_by_name(object_index, marker_name, &marker, 1) )
    {
        const char *marker_names[1] = { marker_name };
        effect_new_attached_from_markers(definition_index, -1, object_index, marker.node_index, 1,
                                         marker_names, (const real_point3d *)&marker.matrix.n[3],
                                         (const real_vector3d *)&marker.matrix.n[0], 1.0f, 1.0f,
                                         nullptr, nullptr);
    }
}
