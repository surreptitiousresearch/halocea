#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"

void clip_position_to_plane(const real_point3d *position, const real_plane3d *plane, real_point3d *result)
{
    float distance = plane->n.n[0] * position->n[0]
                    + plane->n.n[1] * position->n[1]
                    + plane->n.n[2] * position->n[2]
                    - plane->d;

    result->n[0] = position->n[0] - plane->n.n[0] * distance;
    result->n[1] = position->n[1] - plane->n.n[1] * distance;
    result->n[2] = position->n[2] - plane->n.n[2] * distance;
}
