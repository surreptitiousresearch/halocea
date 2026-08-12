/* weather_particle_system_build_clipping_planes @0x8373C608 — build the 5-plane clip volume weather
 * particles are culled against: the 4 side/near frustum planes are copied straight from the render
 * frustum, and a 5th far-clip plane is synthesized through the point `camera position + camera forward *
 * far_clip_distance`, facing back along the camera's forward vector. */

#include "headers/real_plane3d.h"
#include "headers/render_globals.h"
#include "headers/clipping_plane.h"
#include "headers/blam_data_globals.h"


void weather_particle_system_build_clipping_planes(real_plane3d *clip_planes, float far_clip_distance)
{
    real_point3d far_point;
    far_point.n[0] = render.camera.forward.n[0] * far_clip_distance + render.camera.position.n[0];
    far_point.n[1] = render.camera.forward.n[1] * far_clip_distance + render.camera.position.n[1];
    far_point.n[2] = render.camera.forward.n[2] * far_clip_distance + render.camera.position.n[2];

    clip_planes[_clip_far].normal = render.camera.forward;
    clip_planes[_clip_far].distance = clip_planes[_clip_far].normal.n[0] * far_point.n[0]
                                     + clip_planes[_clip_far].normal.n[1] * far_point.n[1]
                                     + clip_planes[_clip_far].normal.n[2] * far_point.n[2];

    clip_planes[0] = render.frustum.world_planes[0];
    clip_planes[1] = render.frustum.world_planes[1];
    clip_planes[2] = render.frustum.world_planes[2];
    clip_planes[3] = render.frustum.world_planes[3];
}
