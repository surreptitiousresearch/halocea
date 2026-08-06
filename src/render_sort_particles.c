/* render_sort_particles @0x83821240 — sorts render_particles' flat rendered-particle list so runs sharing
 * (definition_index, cluster_index, attached_to_first_person_weapon) become contiguous, ready for the
 * caller's group-collapsing pass.
 *
 * DEVIATION — the DB's own body is a direct call into the compiled STL template
 * `std::_Sort<rendered_particle_datum*, int, compare_rendered_particles>` (introsort: quicksort + heapsort
 * fallback + final insertion sort, ~17 separate template instantiations in the DB). That machinery is
 * generic C++ standard-library boundary code, not hand-written Blam logic, so it is not reproduced here;
 * instead this uses the standard C `qsort` with compare_rendered_particles.c's comparator, which reproduces
 * the STL predicate's exact 3-key ordering. `qsort` is not a stable sort any more than `std::sort` is, so
 * this preserves the original's semantics (a strict weak ordering, no stability guarantee) rather than just
 * its results. */

#include "headers/rendered_particle_datum.h"

extern void qsort(void *base, unsigned int count, unsigned int size,
        int (*compare)(const void *, const void *));
extern int compare_rendered_particles(const void *left, const void *right);

void render_sort_particles(rendered_particle_datum *rendered_particles, unsigned int rendered_particles_count)
{
    qsort(rendered_particles, rendered_particles_count, sizeof(rendered_particle_datum), compare_rendered_particles);
}
