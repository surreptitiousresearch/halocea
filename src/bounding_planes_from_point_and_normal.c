/* bounding_planes_from_point_and_normal @ 0x836E6370
   Builds a pair of parallel bounding planes from a point and a normal. The
   lower-bound plane has the given normal and passes through the point; the
   upper-bound plane is its negation (opposite normal, negated distance). */

#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"

void bounding_planes_from_point_and_normal(const real_vector3d *n, const real_point3d *p,
    real_plane3d *lower_bound, real_plane3d *upper_bound)
{
    lower_bound->n.n[0] = n->n[0];
    lower_bound->n.n[1] = n->n[1];
    lower_bound->n.n[2] = n->n[2];
    lower_bound->d = n->n[0] * p->n[0] + (n->n[2] * p->n[2] + n->n[1] * p->n[1]);

    upper_bound->n.n[0] = -lower_bound->n.n[0];
    upper_bound->n.n[1] = -lower_bound->n.n[1];
    upper_bound->n.n[2] = -lower_bound->n.n[2];
    upper_bound->d = -lower_bound->d;
}
