/* structure_visibility_find_objects @0x837C4DA8 — collect the objects visible this render pass. Walks every
 * rendered cluster, enumerates the objects it holds via the supplied cluster iterator callbacks, and for each
 * not-yet-marked object whose bounding sphere passes the cluster frustum test, records its index and marks it
 * (so an object spanning several clusters is only emitted once). Returns the number collected. */

#include <stdint.h>
#include "headers/render_globals.h"
#include "headers/real_point3d.h"
#include "headers/render_frustum.h"

#include "headers/render_frustum.h"
extern int16_t render_frustum_sphere_visible(const render_frustum *frustum, const real_point3d *point, float radius);

/* DEVIATION (2026-08-12, #134): the unmarked/mark slots are `int (*)(int object_index)`. This REFUTES
 * the 2026-07-30 note that stood here, which narrowed them to `uint16_t` because "all four attested
 * impls are int(uint16_t) (clrlwi16 param first-use)" — and the four impls had in turn been narrowed
 * by citing each other (object_mark_function.c carried "matches its three visibility-callback
 * siblings"). Four functions narrowed in a circle, on the first-use mask that #130 showed is
 * DATA_ARRAY_ELEMENT's own truncation (data_array.h), not an ABI width.
 *
 * The binary decides it here, in this function's own body @0x837C4DA8:
 *     837C4E10  bctrl                       ; cluster_get_first -> index in r3
 *     837C4E14  mr     r31, r3
 *     837C4E18  cmpwi  cr6, r31, -1         ; a full 32-bit sentinel test
 *     837C4E20  mr     r3, r31
 *     837C4E28  bctrl                       ; unmarked(r31)  -- the WHOLE word
 *     837C4E44  mr     r3, r31
 *     837C4E48  bctrl                       ; get_bounding_sphere(int, ...) -- same r31
 *     837C4E98  stwx   r31, r11, r25        ; stored WHOLE into `int *result_indices`
 *     837C4E9C  bctrl                       ; mark(r31)      -- the WHOLE word
 * One value reaches an `int` array, an `int` callback parameter and both mark callbacks. It travels
 * at 32 bits; only two of the five slots claimed otherwise. */
int16_t structure_visibility_find_objects(int *result_indices, int16_t maximum_count,
    int (*cluster_get_first)(int *, int16_t), int (*cluster_get_next)(int *),
    void (*get_bounding_sphere)(int, real_point3d *, float *), int (*unmarked)(int object_index),
    int (*mark)(int object_index))
{
    int16_t found_count = 0;
    if (render.rendered_cluster_count <= 0)
        return found_count;

    for (int cluster = 0; cluster < render.rendered_cluster_count; cluster = (int16_t)(cluster + 1))
    {
        int iterator;
        for (int object_index = cluster_get_first(&iterator, render.rendered_clusters[cluster].cluster_index);
             object_index != -1;
             object_index = cluster_get_next(&iterator))
        {
            if (!unmarked(object_index))
                continue;

            real_point3d bounding_sphere_center;
            float bounding_sphere_radius;
            get_bounding_sphere(object_index, &bounding_sphere_center, &bounding_sphere_radius);

            int16_t insert_index = found_count;
            if (insert_index < maximum_count
                && (render.cluster_index == -1
                    || render_frustum_sphere_visible(&render.rendered_clusters[cluster].frustum,
                                                     &bounding_sphere_center, bounding_sphere_radius)))
            {
                found_count = (int16_t)(found_count + 1);
                result_indices[insert_index] = object_index;
                mark(object_index);
            }
        }
    }

    return found_count;
}
