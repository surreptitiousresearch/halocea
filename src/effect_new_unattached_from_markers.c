/* effect_new_unattached_from_markers @0x836E4238 — spawn an effect not attached to any object, placing
 * its event markers at the supplied world points/forwards. Allocates the effect datum, fills in scale,
 * color (defaulting to white), optional impulse vector field and velocity, resolves its BSP location,
 * publishes the marker list for effect_build_locations, then runs the first effect_update.
 *
 * The decompiler over-counted the argument list (spurious a13..a35); the true signature is the 12-arg
 * form below — color, impulse_field and deterministic are the trailing three. */

#include <stdint.h>
#include <string.h>
#include "headers/effect_datum.h"
#include "headers/effect_marker_list.h"
#include "headers/effect_vector_field.h"
#include "headers/object_marker.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
#include "headers/object_marker.h"
#include "headers/object_marker.h"
#include "headers/object_marker.h"
#include "headers/data_array.h"


extern void hcex_init_effect(int definition_index, int obj_follow, int plr_idx, real_point3d *points,
        real_vector3d *vectors, const char **names, int npoints, float scale);
extern void *datum_get(data_array *data, int index);
extern int effect_allocate(int definition_index, int owner_object_index, uint8_t can_be_deterministic);
extern void scenario_location_from_point(location *location, const real_point3d *point);
extern void effect_build_locations(effect_datum *effect, int16_t (*get_markers_by_name)(int, const char *, object_marker *, int16_t));
extern int16_t effect_marker_list_get_markers_by_name(int object_index, const char *name, object_marker *results, int16_t maximum_count);
extern void effect_update(int effect_index, float dt);

extern void impulse_effect_initialize(effect_datum *effect, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);
int effect_new_unattached_from_markers(int definition_index, int owner_object_index,
        const real_vector3d *translational_velocity, int16_t marker_count, const char **marker_names,
        real_point3d *marker_points, real_vector3d *marker_forwards, float scale_a, float scale_b,
        const real_rgb_color *color, const effect_vector_field *impulse_field, uint8_t deterministic)
{
    int effect_index;
    effect_datum *effect;
    const real_vector3d *velocity;
    effect_marker_list markers;

    hcex_init_effect(definition_index, -1, -1, marker_points, marker_forwards, marker_names, marker_count, scale_a);
    effect_index = effect_allocate(definition_index, owner_object_index, deterministic);
    if ( effect_index == -1 )
        return effect_index;

    /* recovered: effect_data->data + 252*(u16)effect_index -> datum_get (252 == effect datum_size incl. 2-byte salt header) */
    effect = datum_get(effect_data, effect_index);
    /* DEVIATION: collapsed verbatim-inlined body of impulse_effect_initialize@0x836E15E0 (zero-xref
     * donor) back into its call; the now-dead `resolved_color` clone-local was dropped. */
    impulse_effect_initialize(effect, scale_a, scale_b, color, impulse_field);
    effect->object_index = -1;

    markers.node_index = -1;
    markers.node_matrix = 0;
    markers.count = marker_count;
    markers.names = marker_names;
    markers.points = marker_points;
    markers.forwards = marker_forwards;

    scenario_location_from_point(&effect->location, marker_points);

    velocity = translational_velocity ? translational_velocity : global_zero_vector3d;
    effect->velocity.n[0] = velocity->n[0];
    global_effect_marker_list = &markers;
    effect->velocity.n[1] = velocity->n[1];
    effect->velocity.n[2] = velocity->n[2];

    memset(effect->location_datum_indices, -1, sizeof(effect->location_datum_indices));
    effect_build_locations(effect, effect_marker_list_get_markers_by_name);
    effect_update(effect_index, 0.0);
    return effect_index;
}
