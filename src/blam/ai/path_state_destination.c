/* path_state_destination @0x837D24E8 — stages a path_state's target point, acceptance radius, and
 * surface index before path_state_find/path_state_build_path are run.
 *
 * DEVIATION: destination_accept_radius is a genuine float (f2); the decompiler widened it to double per
 * its usual PPC argument-promotion artifact. */

#include "headers/path_state.h"
#include "headers/real_point3d.h"

void path_state_destination(path_state *state, const real_point3d *destination_point,
        int destination_surface_index, float destination_accept_radius)
{
    state->destination_valid = 1;
    state->destination.point.n[0] = destination_point->n[0];
    state->destination.point.n[1] = destination_point->n[1];
    state->destination.point.n[2] = destination_point->n[2];
    state->destination.target_radius = destination_accept_radius;
    state->destination.surface_index = destination_surface_index;
}
