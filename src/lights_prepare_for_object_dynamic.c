/* lights_prepare_for_object_dynamic @0x836F9F78 — choose the point lights that illuminate an object this frame.
 * Starting from the object's bounding sphere (center + radius), it walks every BSP cluster the object touches
 * and lets find_point_lights_for_object_in_cluster compete lights into the lighting block's point-light slots.
 * The marker counter is bumped so each light is considered at most once across clusters. Afterwards each stored
 * slot, which holds a light datum index, is resolved to that light's runtime render index (light datum +0x08).
 *
 * DEVIATION: the decompiler renders the point-light slots through the ambient_color float array (union overlay)
 * and passes a junk register for find_point_lights' vestigial 5th argument; reconstructed against the named
 * render_lighting fields and the verified find_point_lights signature. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/render_lighting.h"
#include "headers/object_cluster_iterator.h"
#include "headers/lights_globals.h"
#include "headers/light_datum.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"

#define LIGHT_DATUM_SIZE 124
#define POINT_LIGHTS_PER_OBJECT 2


extern int16_t object_get_first_cluster(object_cluster_iterator *iterator, int object_index);
extern int16_t object_get_next_cluster(object_cluster_iterator *iterator, int object_index);
extern void find_point_lights_for_object_in_cluster(int object_index, int16_t cluster_index, const real_point3d *point, float radius, int *vestigial_unused, int *selected_light_indices, float *light_priorities, float *light_falloffs, int16_t *light_count, int16_t maximum_count);

extern void light_marker_begin(void);
void lights_prepare_for_object_dynamic(int object_index, render_lighting *lighting)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    real_point3d center;
    center.n[0] = object->object.bounding_sphere_center.n[0];
    center.n[1] = object->object.bounding_sphere_center.n[1];
    center.n[2] = object->object.bounding_sphere_center.n[2];
    float radius = object->object.bounding_sphere_radius;

    lighting->point_light_count = 0;
    /* DEVIATION: inlined body of light_marker_begin@0x836F84D8 collapsed to call (donor takes no args, nothing to fold) */
    light_marker_begin();

    float light_priorities[POINT_LIGHTS_PER_OBJECT];
    float light_falloffs[POINT_LIGHTS_PER_OBJECT];
    object_cluster_iterator iterator;
    for ( int16_t cluster = object_get_first_cluster(&iterator, object_index);
          cluster != -1;
          cluster = object_get_next_cluster(&iterator, object_index) )
    {
        find_point_lights_for_object_in_cluster(object_index, cluster, &center, radius,
                                                0, lighting->point_light_indices,
                                                light_priorities, light_falloffs,
                                                &lighting->point_light_count, POINT_LIGHTS_PER_OBJECT);
    }

    lights_globals.marker_initialized = 0;

    /* resolve each gathered light datum index to its runtime render index (light datum +0x08) */
    for ( int i = 0; i < lighting->point_light_count; i++ )
        lighting->point_light_indices[i] = DATA_ARRAY_ELEMENT(light_data, light_datum,
                                            lighting->point_light_indices[i])->rasterizer_light_index;
}
