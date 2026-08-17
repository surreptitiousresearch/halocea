/* projectile_calculate_deceleration_from_distances @0x83759DF0 */
#include "headers/projectile_definition.h"

float projectile_calculate_deceleration_from_distances(projectile_definition *projectile_definition,
        float minimum_distance, float maximum_distance)
{
    float distance_delta = maximum_distance - minimum_distance;

    if (projectile_definition->projectile.initial_velocity == projectile_definition->projectile.final_velocity
        || distance_delta == 0.0f)
        return 0.0f;

    float initial_velocity = projectile_definition->projectile.initial_velocity;
    float final_velocity = projectile_definition->projectile.final_velocity;

    return (initial_velocity * initial_velocity - final_velocity * final_velocity) / (distance_delta * 2.0f);
}
