/* compare_rendered_particles::operator() @0x83820330 — qsort-style comparator ordering rendered_particle_datum
 * entries by (definition_index, cluster_index, attached_to_first_person_weapon), used by render_sort_particles
 * to group same-definition/same-cluster/same-attachment particles into contiguous runs. The DB's own body
 * returns a `bool` "particle0 < particle1" (STL strict-weak-order predicate, `v5 >> 31` is a sign-bit less-than
 * test on the definition_index/cluster_index difference) — translated to a 3-way qsort comparator here rather
 * than a bool predicate, since it's called through `qsort` (see render_sort_particles.c) instead of the DB's
 * original `std::_Sort` template. */

#include "headers/rendered_particle_datum.h"

int compare_rendered_particles(const void *left, const void *right)
{
    const rendered_particle_datum *particle0 = (const rendered_particle_datum *)left;
    const rendered_particle_datum *particle1 = (const rendered_particle_datum *)right;

    if (particle0->definition_index != particle1->definition_index)
        return particle0->definition_index - particle1->definition_index;
    if (particle0->cluster_index != particle1->cluster_index)
        return particle0->cluster_index - particle1->cluster_index;
    return (int)particle0->attached_to_first_person_weapon - (int)particle1->attached_to_first_person_weapon;
}
