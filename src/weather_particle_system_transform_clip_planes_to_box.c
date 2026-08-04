/* weather_particle_system_transform_clip_planes_to_box @0x8373C700 — projects each of the 5 weather
 * clip planes' normals onto box_offset, producing one scalar transform per plane. */

#include "headers/real_plane3d.h"
#include "headers/real_point3d.h"

void weather_particle_system_transform_clip_planes_to_box(float *clip_plane_d_transforms,
        const real_plane3d *clip_planes, real_point3d *box_offset)
{
    for (int i = 0; i < 5; i++)
    {
        clip_plane_d_transforms[i] = clip_planes[i].normal.n[0] * box_offset->n[0]
                                    + clip_planes[i].normal.n[1] * box_offset->n[1]
                                    + clip_planes[i].normal.n[2] * box_offset->n[2];
    }
}
