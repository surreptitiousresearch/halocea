/* line_from_planes3d @0x837000B8 — the line where two planes intersect: direction is their normals'
 * cross product; if the planes are (nearly) parallel (|direction|^2 < 0.0001), returns 0 with no output.
 * Otherwise returns 1 with `vector` set to the direction and `point` set to the point on that line closest
 * to the origin. */

#include <stdint.h>
#include "headers/real_plane3d.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern float fabsf(float value);

/* attested: return is uint8_t (li r3,1 @8370013C / li r3,0 @837001D4; both
 * collision_move_point call sites re-normalize with clrlwi r3,24). */
uint8_t line_from_planes3d(const real_plane3d *plane0, const real_plane3d *plane1, real_point3d *point,
    real_vector3d *vector)
{
    float n0x = plane0->n.n[0], n0y = plane0->n.n[1], n0z = plane0->n.n[2];
    float n1x = plane1->n.n[0], n1y = plane1->n.n[1], n1z = plane1->n.n[2];

    float direction_x = n0y * n1z - n0z * n1y;
    float direction_y = n0z * n1x - n0x * n1z;
    float direction_z = n0x * n1y - n0y * n1x;
    vector->n[0] = direction_x;
    vector->n[1] = direction_y;
    vector->n[2] = direction_z;

    float direction_length_squared = direction_z * direction_z + direction_x * direction_x + direction_y * direction_y;
    if ( fabsf(direction_length_squared) < 0.0001f )
        return 0;

    float d0 = plane0->d;
    float d1 = plane1->d;

    float a = n1x * direction_y - n1y * direction_x;
    float b = n1z * direction_x - n1x * direction_z;
    float c = n1y * direction_z - n1z * direction_y;

    float inverse_length_squared = 1.0f / direction_length_squared;

    point->n[2] = ((n0y * direction_x - direction_y * n0x) * d1 + a * d0) * inverse_length_squared;
    point->n[0] = ((n0z * direction_y - n0y * direction_z) * d1 + c * d0) * inverse_length_squared;
    point->n[1] = (d1 * (direction_z * n0x - n0z * direction_x) + d0 * b) * inverse_length_squared;

    return 1;
}
