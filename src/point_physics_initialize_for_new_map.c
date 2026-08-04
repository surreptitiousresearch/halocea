#include "headers/blam_data_globals.h"
/* point_physics_initialize_for_new_map @0x836E0618 — precompute the air/water mass-over-radius-cubed drag
 * constants for the current map from the global air/water densities. */

extern float global_air_mass_over_radius_cubed;     /* global_air_mass_over_radius_cubed */

void point_physics_initialize_for_new_map(void)
{
    global_air_mass_over_radius_cubed = (float)(global_air_density * 118613.34);
    global_water_mass_over_radius_cubed = (float)(global_water_density * 118613.34);
}
