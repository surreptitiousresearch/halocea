/* line_from_planes3d @0x837000B8 — the line where two planes intersect: direction is their normals'
 * cross product; if the planes are (nearly) parallel (|direction|^2 < 0.0001), returns 0 with no output.
 * Otherwise returns 1 with `vector` set to the direction and `point` set to the point on that line closest
 * to the origin. */

#include <stdint.h>
#include "headers/real_plane3d.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/fused_math.h"

extern float fabsf(float value);

/* attested: return is uint8_t (li r3,1 @8370013C / li r3,0 @837001D4; both
 * collision_move_point call sites re-normalize with clrlwi r3,24). */
uint8_t line_from_planes3d(const real_plane3d *plane0, const real_plane3d *plane1, real_point3d *point,
    real_vector3d *vector)
{
    float n0x = plane0->n.n[0], n0y = plane0->n.n[1], n0z = plane0->n.n[2];
    float n1x = plane1->n.n[0], n1y = plane1->n.n[1], n1z = plane1->n.n[2];

    /* DEVIATION: fused cross product — fmuls @837000C8/837000D8/837000E0, fmsubs @837000E8/837000F0/837000F8 */
    float direction_y = fused_msub(n0z, n1x, n1z * n0x);
    float direction_x = fused_msub(n1z, n0y, n0z * n1y);
    float direction_z = fused_msub(n0x, n1y, n1x * n0y);
    vector->n[0] = direction_x;
    vector->n[1] = direction_y;
    vector->n[2] = direction_z;

    /* DEVIATION: fused length dot — fmuls @83700100, fmadds @83700108/8370010C */
    float direction_length_squared = fused_madd(direction_z, direction_z,
        fused_madd(direction_x, direction_x, direction_y * direction_y));
    if ( fabsf(direction_length_squared) < 0.0001f )
        return 0;

    float d0 = plane0->d;
    float d1 = plane1->d;

    /* DEVIATION: fused closest-point construction — fmuls @83700128/83700130/83700134,
     * fmsubs @83700148/8370014C/83700150 (plane1 stage), fmuls @83700158/83700160/83700168,
     * fmuls @8370018C/83700198/8370019C, fmsubs @837001A0/837001A4/837001A8,
     * fmadds @837001AC/837001B0/837001B4 (plane0 stage), fmuls @837001B8/837001C0/837001C8 */
    float a = fused_msub(n1x, direction_y, n1y * direction_x);
    float b = fused_msub(n1z, direction_x, n1x * direction_z);
    float c = fused_msub(n1y, direction_z, n1z * direction_y);

    float inverse_length_squared = 1.0f / direction_length_squared;

    float accumulator_z = a * d0;
    float accumulator_y = d0 * b;
    float accumulator_x = c * d0;

    accumulator_z = fused_madd(fused_msub(n0y, direction_x, direction_y * n0x), d1, accumulator_z);
    accumulator_x = fused_madd(fused_msub(n0z, direction_y, n0y * direction_z), d1, accumulator_x);
    accumulator_y = fused_madd(d1, fused_msub(direction_z, n0x, n0z * direction_x), accumulator_y);

    point->n[2] = accumulator_z * inverse_length_squared;
    point->n[0] = accumulator_x * inverse_length_squared;
    point->n[1] = accumulator_y * inverse_length_squared;

    return 1;
}
