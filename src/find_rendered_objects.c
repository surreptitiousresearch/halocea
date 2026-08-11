/* find_rendered_objects @0x836E5D60 — build the list of objects visible this render pass. Runs the structure
 * visibility query twice (collideable objects first, then noncollideable into the remaining slots), capping
 * at 256 total. */

#include <stdint.h>
#include "headers/render_object_globals.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"

#include "headers/real_point3d.h"
#include "headers/real_point3d.h"
extern void object_marker_begin(void);
extern void object_marker_end(void);
extern int16_t structure_visibility_find_objects(int *result_indices, int16_t maximum_count, int (*cluster_get_first)(int *, int16_t), int (*cluster_get_next)(int *), void (*get_bounding_sphere)(int, real_point3d *, float *), int (*unmarked)(int object_index), int (*mark)(int object_index));

extern int cluster_get_first_collideable_object(int *reference_index, int16_t cluster_index);
extern int cluster_get_next_collideable_object(int *reference_index);
extern int cluster_get_first_noncollideable_object(int *reference_index, int16_t cluster_index);
extern int cluster_get_next_noncollideable_object(int *reference_index);
extern void object_get_render_bounding_sphere(int object_index, real_point3d *center, float *radius);
extern int object_unmarked_function(int object_index);
extern int object_mark_function(int object_index);


void find_rendered_objects(void) /* attested void: object_marker_end is void, 0/1 callers consume r3 */
{
    object_marker_begin();

    render_object_globals.rendered_object_count = structure_visibility_find_objects(
        render_object_globals.rendered_object_indices, 256,
        cluster_get_first_collideable_object, cluster_get_next_collideable_object,
        object_get_render_bounding_sphere, object_unmarked_function, object_mark_function);

    render_object_globals.rendered_object_count += structure_visibility_find_objects(
        &render_object_globals.rendered_object_indices[render_object_globals.rendered_object_count],
        256 - render_object_globals.rendered_object_count,
        cluster_get_first_noncollideable_object, cluster_get_next_noncollideable_object,
        object_get_render_bounding_sphere, object_unmarked_function, object_mark_function);

    object_marker_end();
    if (render_object_globals.rendered_object_count == 256 && !once_0)
        once_0 = 1;
}
