/* biped_approximate_surface_index @0x837AD318 — find the collision surface index beneath a biped by
 * casting straight down (global_down3d) from the biped, with a 2.0 world-unit probe length. Returns the
 * surface index found by biped_find_ground_surface; surface_point receives the contact point. */

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

#include "headers/real_vector3d.h"
extern int biped_find_ground_surface(int biped_index, float ground_distance, const real_vector3d *ground_direction, real_point3d *surface_point, real_vector3d *surface_normal);

int biped_approximate_surface_index(int biped_index, real_point3d *surface_point)
{
    /* Binary-faithful argument order (disasm 0x837AD318..0x837AD330):
     *   r3=biped_index, f1=2.0 (ground_distance), r5=global_down3d (ground_direction),
     *   r6=surface_point, r7=0 (surface_normal). A prior reconstruction had transposed the three
     *   pointer arguments (passed surface_point/global_down3d/surface_point); corrected here. */
    return biped_find_ground_surface(biped_index, 2.0f, global_down3d, surface_point, nullptr);
}
