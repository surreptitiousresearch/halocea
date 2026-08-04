/* rasterizer_transparent_geometry_draw @0x837688B8 — draw the queued transparent geometry groups. For the
 * water pass the groups are first depth-sorted (building a sorted-index table and stamping each group's
 * sorted_index) and the cursor reset; the non-water pass resumes from the current cursor. Each group is drawn
 * in sorted order; the water pass stops at the first non-water / non-water-decal shader. First-person groups
 * (geometry flag 0x80) switch to the first-person frustum range once, restored at the end. */

#include <stdint.h>
#include "headers/transparent_geometry_group.h"
#include "headers/shader.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_geometry_flags.h"
#include "headers/shader_type.h"
#include "headers/blam_data_globals.h"


extern void qsort(void *base, unsigned int count, unsigned int size,
                  int (*compare)(const void *, const void *));
extern int group_sorted_indices_cmpfn(const int16_t *group_index1, const int16_t *group_index2);
extern void rasterizer_transparent_geometry_groups_begin(void);
extern void rasterizer_transparent_geometry_group_draw(const transparent_geometry_group *group, uint8_t dirty);
extern void rasterizer_transparent_geometry_groups_end(void);
extern uint8_t shader_is_water_decal(const shader *shader);
extern void rasterizer_set_stencil_mode(int16_t stencil_mode);
extern void rasterizer_set_frustum_z(float z_near, float z_far);

void rasterizer_transparent_geometry_draw(uint8_t water)
{
    if (transparent_geometry_group_count <= 0)
        return;

    unsigned char first_person_frustum_active = 0;

    if (water)
    {
        for (int i = 0; i < transparent_geometry_group_count; i = (__int16)(i + 1))
            transparent_geometry_group_sorted_indices[i] = i;

        qsort(transparent_geometry_group_sorted_indices, transparent_geometry_group_count, 2,
              (int (*)(const void *, const void *))group_sorted_indices_cmpfn);

        for (int i = 0; i < transparent_geometry_group_count; i = (__int16)(i + 1))
            transparent_geometry_groups[transparent_geometry_group_sorted_indices[i]].sorted_index = i;

        group_index = 0;
    }

    /* FINDINGS: the reconstruction invented a `const shader*` return from the void
       groups_begin() (and a matching arg to groups_end) — decompiler r3-residue typing.
       shader is a genuine local, only ever assigned from group->shader in the water branch. */
    const shader *shader = 0;
    rasterizer_transparent_geometry_groups_begin();

    while (group_index < transparent_geometry_group_count)
    {
        transparent_geometry_group *group =
            &transparent_geometry_groups[transparent_geometry_group_sorted_indices[group_index]];

        if (water)
        {
            shader = group->shader;
            if (!shader)
                break;
            if (shader->base.type != _shader_type_transparent_water && !shader_is_water_decal(shader))
                break;
        }

        if ((group->geometry_flags & (1u << _rasterizer_geometry_first_person_bit)) != 0 && !first_person_frustum_active)
        {
            rasterizer_set_stencil_mode(0);
            rasterizer_set_frustum_z(rasterizer_globals.z_near_first_person,
                                     rasterizer_globals.z_far_first_person);
            first_person_frustum_active = 1;
        }

        rasterizer_transparent_geometry_group_draw(group, 0);
        ++group_index;
    }

    rasterizer_transparent_geometry_groups_end();
    if (first_person_frustum_active)
        rasterizer_set_frustum_z(0.0f, 0.0f);
}
