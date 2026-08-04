/* point_physics_definition_get_mass @0x836E0658 — mass of a point of the given radius:
 * runtime_mass_over_radius_cubed * radius^3. */

#include "headers/point_physics_definition.h"

float point_physics_definition_get_mass(const point_physics_definition *physics, float radius)
{
    return (((physics->runtime_mass_over_radius_cubed * radius) * radius) * radius);
}
