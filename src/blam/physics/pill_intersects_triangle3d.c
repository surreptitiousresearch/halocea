/* pill_intersects_triangle3d @0x836FFC40 — test whether a pill (capsule: base, height axis, width radius)
 * intersects triangle (p0,p1,p2). Projects the pill axis onto the triangle's normal to find the axis point
 * nearest the triangle's plane (t = dot(n, p0-base)/dot(n, height), clamped to [0,1]); then, using the
 * normal-signed edge functions of that point, checks each edge the point falls outside: the pill intersects
 * only if that edge segment pierces it (vector_intersects_pill3d). If the point projects inside all three
 * edges, the pill intersects when the unclamped t lies strictly inside (0,1) (the axis crosses the plane), or
 * otherwise when the clamped endpoint is within `width` of the plane (squared signed plane distance vs
 * |normal|^2 * width^2). Returns 1 on intersection. Pill sibling of sphere_intersects_triangle3d.
 *
 * DEVIATION: the DB prototype carries a phantom 4th parameter — the float `width` (arg 3) burns the r5 GPR
 * shadow slot, so the decompiler shifted the triangle vertices by one and fabricated an unused first vertex;
 * the real vertices are the decompiler's p1/p2/a7, renamed here p0/p1/p2 (same trap as the sphere sibling).
 * Register-pun doubles are single-precision float math throughout; reproduced with float. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern uint8_t vector_intersects_pill3d(const real_point3d *point, const real_vector3d *vector, const real_point3d *base, const real_vector3d *height, float width);

uint8_t pill_intersects_triangle3d(const real_point3d *base, const real_vector3d *height, float width,
        const real_point3d *p0, const real_point3d *p1, const real_point3d *p2)
{
    real_vector3d edge01;
    edge01.n[0] = p1->n[0] - p0->n[0];
    edge01.n[1] = p1->n[1] - p0->n[1];
    edge01.n[2] = p1->n[2] - p0->n[2];

    real_vector3d edge12;
    edge12.n[0] = p2->n[0] - p1->n[0];
    edge12.n[1] = p2->n[1] - p1->n[1];
    edge12.n[2] = p2->n[2] - p1->n[2];

    /* triangle normal = edge01 x edge12 */
    float nx = edge12.n[2] * edge01.n[1] - edge12.n[1] * edge01.n[2];
    float ny = edge12.n[0] * edge01.n[2] - edge12.n[2] * edge01.n[0];
    float nz = edge01.n[0] * edge12.n[1] - edge12.n[0] * edge01.n[1];

    /* axis point nearest the triangle plane: t = dot(n, p0 - base) / dot(n, height), clamped to [0,1] */
    float t = (nx * (p0->n[0] - base->n[0])
            + (nz * (p0->n[2] - base->n[2]) + ny * (p0->n[1] - base->n[1])))
            / (height->n[0] * nx + (height->n[1] * ny + height->n[2] * nz));
    float t_clamped;
    if ( t >= 0.0f )
        t_clamped = t <= 1.0f ? t : 1.0f;
    else
        t_clamped = 0.0f;

    float px = height->n[0] * t_clamped + base->n[0];
    float py = height->n[1] * t_clamped + base->n[1];
    float pz = height->n[2] * t_clamped + base->n[2];

    uint8_t outside = 0;

    /* edge p0->p1 */
    float dx0 = px - p0->n[0];
    float dy0 = py - p0->n[1];
    float dz0 = pz - p0->n[2];
    if ( (dz0 * edge01.n[1] - dy0 * edge01.n[2]) * nx
       + ((dy0 * edge01.n[0] - dx0 * edge01.n[1]) * nz
        + (dx0 * edge01.n[2] - dz0 * edge01.n[0]) * ny) < 0.0f )
    {
        if ( vector_intersects_pill3d(p0, &edge01, base, height, width) )
            return 1;
        outside = 1;
    }

    /* edge p1->p2 */
    float dx1 = px - p1->n[0];
    float dy1 = py - p1->n[1];
    float dz1 = pz - p1->n[2];
    if ( (dz1 * edge12.n[1] - dy1 * edge12.n[2]) * nx
       + ((dy1 * edge12.n[0] - dx1 * edge12.n[1]) * nz
        + (edge12.n[2] * dx1 - dz1 * edge12.n[0]) * ny) < 0.0f )
    {
        if ( vector_intersects_pill3d(p1, &edge12, base, height, width) )
            return 1;
        outside = 1;
    }

    /* edge p2->p0 */
    real_vector3d edge20;
    edge20.n[0] = p0->n[0] - p2->n[0];
    edge20.n[1] = p0->n[1] - p2->n[1];
    edge20.n[2] = p0->n[2] - p2->n[2];
    float dx2 = px - p2->n[0];
    float dy2 = py - p2->n[1];
    float dz2 = pz - p2->n[2];
    if ( (edge20.n[1] * dz2 - edge20.n[2] * dy2) * nx
       + ((dy2 * edge20.n[0] - edge20.n[1] * dx2) * nz
        + (edge20.n[2] * dx2 - dz2 * edge20.n[0]) * ny) < 0.0f )
    {
        if ( vector_intersects_pill3d(p2, &edge20, base, height, width) )
            return 1;
        outside = 1;
    }

    if ( outside )
        return 0;

    /* inside all edges: the axis crosses the plane within the segment */
    if ( t > 0.0f && t < 1.0f )
        return 1;

    /* clamped endpoint: within width of the triangle plane */
    float plane_distance = nx * dx2 + (nz * dz2 + ny * dy2);
    return plane_distance * plane_distance
        <= ((nx * nx + (nz * nz + ny * ny)) * width) * width;
}
