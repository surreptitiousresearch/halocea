/* sphere_intersects_triangle3d @0x836FF498 — test whether a sphere (center, radius) intersects triangle
 * (p0,p1,p2). First rejects when the sphere is farther from the triangle's plane than its radius (compares the
 * squared signed plane distance against |normal|^2 * radius^2). Then, using the triangle-normal-signed edge
 * functions of the sphere center, decides which region the center projects into: if it falls outside an edge,
 * the sphere only intersects when that edge segment pierces it (fast_vector_intersects_sphere); if the center
 * projects inside all three edges, the plane test already established a hit. Returns 1 on intersection.
 *
 * DEVIATION: the DB prototype is 5-arg (center, radius, p0, p1, p2); the float `radius` (arg 2) burns a GPR
 * shadow slot, so the decompiler shifted the triangle vertices by one and fabricated a phantom first vertex —
 * the real triangle vertices are the decompiler's p1/p2/a6, renamed here p0/p1/p2. Register-pun doubles are
 * single-precision float math (all subexpressions cast to float in the original); reproduced with float. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern uint8_t fast_vector_intersects_sphere(const real_point3d *point, const real_vector3d *vector, const real_point3d *center, float radius);

uint8_t sphere_intersects_triangle3d(const real_point3d *center, float radius, const real_point3d *p0,
        const real_point3d *p1, const real_point3d *p2)
{
    real_vector3d edge01;
    edge01.n[0] = p1->n[0] - p0->n[0];
    edge01.n[1] = p1->n[1] - p0->n[1];
    edge01.n[2] = p1->n[2] - p0->n[2];

    real_vector3d edge12;
    edge12.n[0] = p2->n[0] - p1->n[0];
    edge12.n[1] = p2->n[1] - p1->n[1];
    edge12.n[2] = p2->n[2] - p1->n[2];

    float cx0 = center->n[0] - p0->n[0];
    float cy0 = center->n[1] - p0->n[1];
    float cz0 = center->n[2] - p0->n[2];

    /* triangle normal = edge01 x edge12 */
    float nx = edge01.n[2] * edge12.n[0] - edge12.n[2] * edge01.n[0];
    float ny = edge12.n[1] * edge01.n[0] - edge12.n[0] * edge01.n[1];
    float nz = edge12.n[2] * edge01.n[1] - edge12.n[1] * edge01.n[2];

    float plane_distance = nz * cx0 + (ny * cz0 + nx * cy0);

    /* sphere too far from the triangle plane */
    if ( plane_distance * plane_distance > ((nz * nz + (ny * ny + nx * nx)) * radius) * radius )
        return 0;

    uint8_t result = 1;

    /* edge p0->p1 */
    if ( (edge01.n[2] * cy0 - edge01.n[1] * cz0) * nz
       + ((cx0 * edge01.n[1] - edge01.n[0] * cy0) * ny
        + (edge01.n[0] * cz0 - edge01.n[2] * cx0) * nx) > 0.0f )
    {
        if ( fast_vector_intersects_sphere(p0, &edge01, center, radius) )
            return 1;
        result = 0;
    }

    /* edge p1->p2 */
    float cx1 = center->n[0] - p1->n[0];
    float cy1 = center->n[1] - p1->n[1];
    float cz1 = center->n[2] - p1->n[2];
    if ( (edge12.n[2] * cy1 - edge12.n[1] * cz1) * nz
       + ((edge12.n[1] * cx1 - edge12.n[0] * cy1) * ny
        + (edge12.n[0] * cz1 - edge12.n[2] * cx1) * nx) > 0.0f )
    {
        if ( fast_vector_intersects_sphere(p1, &edge12, center, radius) )
            return 1;
        result = 0;
    }

    /* edge p2->p0 */
    real_vector3d edge20;
    edge20.n[0] = p0->n[0] - p2->n[0];
    edge20.n[1] = p0->n[1] - p2->n[1];
    edge20.n[2] = p0->n[2] - p2->n[2];
    float cx2 = center->n[0] - p2->n[0];
    float cy2 = center->n[1] - p2->n[1];
    float cz2 = center->n[2] - p2->n[2];
    if ( (edge20.n[2] * cy2 - edge20.n[1] * cz2) * nz
       + ((edge20.n[1] * cx2 - edge20.n[0] * cy2) * ny
        + (edge20.n[0] * cz2 - edge20.n[2] * cx2) * nx) < 0.0f )
        return fast_vector_intersects_sphere(p2, &edge20, center, radius) != 0;

    return result;
}
