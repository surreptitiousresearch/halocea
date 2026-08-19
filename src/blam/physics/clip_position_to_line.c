/* clip_position_to_line @0x837743C8 — project `position` onto the line through `point` along `vector`,
 * writing the closest point on the line into `result`. Same "distance / dot(vector,vector)" projection
 * shape as clip_position_to_plane.c, cleaned up from the decompiler's deeply-nested duplicated expression. */

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

void clip_position_to_line(const real_point3d *position, const real_point3d *point, const real_vector3d *vector,
    real_point3d *result)
{
    float delta[3] = {
        position->n[0] - point->n[0],
        position->n[1] - point->n[1],
        position->n[2] - point->n[2],
    };
    float numerator = vector->n[0] * delta[0] + vector->n[1] * delta[1] + vector->n[2] * delta[2];
    float denominator = vector->n[0] * vector->n[0] + vector->n[1] * vector->n[1] + vector->n[2] * vector->n[2];
    float t = numerator / denominator;

    result->n[0] = vector->n[0] * t + point->n[0];
    result->n[1] = vector->n[1] * t + point->n[1];
    result->n[2] = vector->n[2] * t + point->n[2];
}
