/* render_camera_triangle_frontfacing @0x8376C7FC — true if triangle (p0,p1,p2) faces the camera: the sign of
 * the scalar triple product of the triangle's edge vectors against the vector from p0 to the camera. */

#include "headers/render_camera.h"
#include "headers/real_point3d.h"

unsigned __int8 render_camera_triangle_frontfacing(const render_camera *camera, const real_point3d *p0,
    const real_point3d *p1, const real_point3d *p2)
{
    float e1x = p1->n[0] - p0->n[0];
    float e1y = p1->n[1] - p0->n[1];
    float e1z = p1->n[2] - p0->n[2];

    float e2x = p2->n[0] - p1->n[0];
    float e2y = p2->n[1] - p1->n[1];
    float e2z = p2->n[2] - p1->n[2];

    float dx = p0->n[0] - camera->position.n[0];
    float dy = p0->n[1] - camera->position.n[1];
    float dz = p0->n[2] - camera->position.n[2];

    float result = ((e1z * e2x - e2z * e1x) * dy)
                  + ((e2y * e1x - e2x * e1y) * dz)
                  + ((e2z * e1y - e2y * e1z) * dx);

    return result > -0.0001f;
}
