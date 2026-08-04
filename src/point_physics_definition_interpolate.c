#include "headers/point_physics_definition.h"

/* point_physics_definition_interpolate — blend two point-physics tags by `t`, copying `physics1`'s
 * flags verbatim and lerping every scalar field. Returns the output pointer (arg3, r6).
 * DEVIATION (2026-07-31): four params, not five. `t` is a float (f1) that also consumes its GPR shadow
 * slot r5, so there is no separate r5 pointer param — the earlier `unused_result` was that float shadow
 * slot misread as an argument. Matches the DB prototype and the caller extern. */
point_physics_definition * point_physics_definition_interpolate(const point_physics_definition *physics1, const point_physics_definition *physics2, float t, point_physics_definition *out)
{
    out->flags = physics1->flags;
    out->density = (physics1->density * (1.0f - t)) + (physics2->density * t);
    out->runtime_water_buoyancy_scale =
        (physics1->runtime_water_buoyancy_scale * (1.0f - t)) + (physics2->runtime_water_buoyancy_scale * t);
    out->runtime_air_buoyancy_scale =
        (physics1->runtime_air_buoyancy_scale * (1.0f - t)) + (physics2->runtime_air_buoyancy_scale * t);
    out->runtime_mass_over_radius_cubed =
        (physics1->runtime_mass_over_radius_cubed * (1.0f - t)) + (physics2->runtime_mass_over_radius_cubed * t);
    out->air_friction = (physics1->air_friction * (1.0f - t)) + (physics2->air_friction * t);
    out->water_friction = (physics1->water_friction * (1.0f - t)) + (physics2->water_friction * t);
    out->contact_friction = (physics1->contact_friction * (1.0f - t)) + (physics2->contact_friction * t);
    out->elasticity = (physics1->elasticity * (1.0f - t)) + (physics2->elasticity * t);

    return out;
}
