/* render_camera_mirror @0x83? — build the virtual camera used to render a mirror's reflection (or refraction)
 * pass. The result starts as a copy of the source camera. For a plain reflector (index_of_refraction == 0) the
 * forward and up vectors are reflected through the mirror plane and the position is mirrored across it; when the
 * camera looks almost edge-on to the plane (|cos| < 0.0125) the reflection normal is nudged toward the view
 * direction and its plane offset recomputed through the camera's projected position, to avoid a degenerate
 * reflection. The handedness flag is flipped and up negated so the mirrored image is not inside-out. For a
 * refractive surface the camera is instead pushed along the plane normal by a refraction offset derived from
 * the incidence angle, the index of refraction and the surface depth. The mirror plane is stored on the result.
 *
 * DEVIATION: the original sets result->mirrored via (_cntlzw(camera->mirrored) & 0x20) != 0, which is the
 * count-leading-zeros idiom for "== 0"; reproduced as a boolean negation. */

#include <string.h>
#include "headers/render_camera.h"
#include "headers/render_mirror.h"
#include "headers/real_vector3d.h"
extern float __fsqrts(float);

extern float normalize3d(real_vector3d *v);
extern real_vector3d *reflect_vector3d(const real_vector3d *incident, const real_vector3d *normal, real_vector3d *reflection);

void render_camera_mirror(const render_camera *camera, const render_mirror *mirror, render_camera *result)
{
    real_vector3d plane_normal = mirror->plane.normal;
    float plane_distance = mirror->plane.distance;

    memcpy(result, camera, sizeof(render_camera));

    if (mirror->index_of_refraction == 0.0f)
    {
        real_vector3d reflection_normal;
        float reflection_distance;

        float facing = camera->forward.n[0] * plane_normal.n[0]
                     + camera->forward.n[2] * plane_normal.n[2]
                     + camera->forward.n[1] * plane_normal.n[1];

        if (__fabs(facing) >= 0.0125f)
        {
            reflection_normal = plane_normal;
            reflection_distance = plane_distance;
        }
        else
        {
            reflection_normal.n[0] = camera->forward.n[0] * 0.005859375f + plane_normal.n[0];
            reflection_normal.n[1] = camera->forward.n[1] * 0.005859375f + plane_normal.n[1];
            reflection_normal.n[2] = camera->forward.n[2] * 0.005859375f + plane_normal.n[2];

            float distance_from_plane = (camera->position.n[0] * plane_normal.n[0]
                                         + camera->position.n[2] * plane_normal.n[2]
                                         + camera->position.n[1] * plane_normal.n[1])
                                        - plane_distance;
            float projected_x = -distance_from_plane * plane_normal.n[0] + camera->position.n[0];
            float projected_y = plane_normal.n[1] * -distance_from_plane + camera->position.n[1];
            float projected_z = plane_normal.n[2] * -distance_from_plane + camera->position.n[2];

            normalize3d(&reflection_normal);
            reflection_distance = reflection_normal.n[0] * projected_x
                                + reflection_normal.n[2] * projected_z
                                + reflection_normal.n[1] * projected_y;
        }

        reflect_vector3d(&camera->forward, &reflection_normal, &result->forward);
        reflect_vector3d(&camera->up, &reflection_normal, &result->up);

        float mirror_scale = ((camera->position.n[0] * reflection_normal.n[0]
                               + camera->position.n[1] * reflection_normal.n[1]
                               + camera->position.n[2] * reflection_normal.n[2])
                              - reflection_distance) * -2.0f;
        result->position.n[0] = reflection_normal.n[0] * mirror_scale + camera->position.n[0];
        result->position.n[1] = reflection_normal.n[1] * mirror_scale + camera->position.n[1];
        result->position.n[2] = reflection_normal.n[2] * mirror_scale + camera->position.n[2];

        result->mirrored = (camera->mirrored == 0);
        result->up.n[0] = -result->up.n[0];
        result->up.n[1] = -result->up.n[1];
        result->up.n[2] = -result->up.n[2];
    }
    else
    {
        float cross_x = camera->forward.n[1] * plane_normal.n[2] - camera->forward.n[2] * plane_normal.n[1];
        float cross_y = camera->forward.n[2] * plane_normal.n[0] - camera->forward.n[0] * plane_normal.n[2];
        float cross_z = camera->forward.n[0] * plane_normal.n[1] - camera->forward.n[1] * plane_normal.n[0];
        float cross_magnitude_squared = cross_x * cross_x + cross_y * cross_y + cross_z * cross_z;

        float forward_magnitude = __fsqrts(camera->forward.n[2] * camera->forward.n[2]
                                           + camera->forward.n[0] * camera->forward.n[0]
                                           + camera->forward.n[1] * camera->forward.n[1]);
        float inverse_forward_magnitude = 1.0f / forward_magnitude;
        float sine_of_incidence = __fsqrts(cross_magnitude_squared) * inverse_forward_magnitude;

        float refraction_offset;
        if (sine_of_incidence == 0.0f)
        {
            refraction_offset = 0.0f;
        }
        else
        {
            float refracted_sine = mirror->index_of_refraction * sine_of_incidence;
            float cosine_of_incidence = (plane_normal.n[1] * camera->forward.n[1]
                                         + camera->forward.n[0] * plane_normal.n[0]
                                         + camera->forward.n[2] * plane_normal.n[2])
                                        * inverse_forward_magnitude;
            refraction_offset = -((cosine_of_incidence * refracted_sine * mirror->depth)
                                  / (__fsqrts(-(refracted_sine * refracted_sine - 1.0f)) * sine_of_incidence));
        }

        result->position.n[0] = plane_normal.n[0] * refraction_offset + camera->position.n[0];
        result->position.n[1] = plane_normal.n[1] * refraction_offset + camera->position.n[1];
        result->position.n[2] = plane_normal.n[2] * refraction_offset + camera->position.n[2];
    }

    result->z_near = 0.0f;
    result->mirror_plane.normal.n[0] = plane_normal.n[0];
    result->mirror_plane.normal.n[1] = plane_normal.n[1];
    result->mirror_plane.normal.n[2] = plane_normal.n[2];
    result->mirror_plane.distance = plane_distance;
}
