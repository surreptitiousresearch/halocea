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

/* unmarked/mark slots respelled u8(*)(int)->int(*)(uint16_t) 2026-07-30: all four attested impls are
 * int(uint16_t) (clrlwi16 param first-use, width-agnostic li 0/1 returns); u8 was the decompiler hint
 * (C4113/C4133 at find_rendered_objects + lights_preprocess_scene) */
__int16 structure_visibility_find_objects(int *result_indices, __int16 maximum_count,
    int (*cluster_get_first)(int *, __int16), int (*cluster_get_next)(int *),
    void (*get_bounding_sphere)(int, real_point3d *, float *), int (*unmarked)(uint16_t),
    int (*mark)(uint16_t))
{
    __int16 found_count = 0;
    if (render.rendered_cluster_count <= 0)
        return found_count;

    for (int cluster = 0; cluster < render.rendered_cluster_count; cluster = (__int16)(cluster + 1))
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

            __int16 insert_index = found_count;
            if (insert_index < maximum_count
                && (render.cluster_index == -1
                    || render_frustum_sphere_visible(&render.rendered_clusters[cluster].frustum,
                                                     &bounding_sphere_center, bounding_sphere_radius)))
            {
                found_count = (__int16)(found_count + 1);
                result_indices[insert_index] = object_index;
                mark(object_index);
            }
        }
    }

    return found_count;
}
