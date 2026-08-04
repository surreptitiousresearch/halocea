/* point_physics_update @0x836E0788 — integrate a free-flying point (particle, contrail point, etc.) for
 * dt against gravity, buoyancy, medium friction, an optional applied force, and the surrounding wind/
 * water current, then sweep it for collision. On collision the point either stops (return with bit set)
 * or reflects with contact friction + elasticity and continues the remaining time, up to 3 bounces.
 *
 * Return value: the medium code in the low bits (1 = air, 2 = water — see medium_result: in_water sets 2,
 * otherwise 1) OR'd with collision bits 4 = hit solid/structure and 8 = hit a non-structure surface.
 *
 * NOTE: the decompiler reordered the trailing arguments (it shows radius/dt ahead of the material-type
 * output and adds a spurious tail). The true signature is the database 11-arg form below; the material
 * type output is the `collision_material_type` parameter. */

#include <stdint.h>
#include "headers/point_physics_definition.h"
#include "headers/point_physics_definition_flags.h"
#include "headers/point_physics_state_flags.h"
#include "headers/collision_result.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result_type.h"
#include "headers/collision_test_flags.h"
#include "headers/blam_data_globals.h"

extern const float global_air_mass_over_radius_cubed;   /* global_air_mass_over_radius_cubed */

extern void scenario_get_current_from_weather_palette(const real_point3d *position, real_vector3d *current_vector, unsigned int flags, int16_t weather_palette_index);
extern uint8_t scenario_get_current(const location *location, const real_point3d *position, real_vector3d *wind_vector, unsigned int flags);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern void component_vectors_from_normal3d(const real_vector3d *vector, const real_vector3d *normal, real_vector3d *parallel, real_vector3d *perpendicular);

unsigned int point_physics_update(unsigned int flags, const point_physics_definition *definition, location *location,
        int16_t force_weather_palette_index, real_point3d *position, real_vector3d *translational_velocity,
        const real_vector3d *translational_force, real_vector3d *collision_normal, int16_t *collision_material_type,
        float radius, float dt)
{
    double remaining = dt;
    float radius_squared = (radius * radius);
    float radius_cubed = ((radius * radius) * radius);
    float mass_over_radius_cubed = definition->runtime_mass_over_radius_cubed;
    unsigned int current_flags;
    uint8_t in_water;
    int medium_result;
    float friction;
    float buoyancy_scale;
    float medium_mass_over_radius_cubed;
    float mass;
    float damping;
    unsigned int collision_flags;
    __int16 bounce;
    real_vector3d current;
    real_vector3d displacement;
    real_vector3d parallel;
    real_vector3d perpendicular;
    collision_result collision;

    if ( dt == 0.0 )
        return 0;

    current_flags = (definition->flags >> _point_physics_simple_wind_bit) & 1 | 2;
    if ( (definition->flags & (1u << _point_physics_damped_wind_bit)) == 0 )
        current_flags = (definition->flags >> _point_physics_simple_wind_bit) & 1;

    if ( (flags & (1u << _point_physics_in_air_bit)) != 0 )
    {
        in_water = (flags >> _point_physics_in_water_bit) & 1;
        scenario_get_current_from_weather_palette(position, &current, current_flags, force_weather_palette_index);
    }
    else
    {
        in_water = scenario_get_current(location, position, &current, current_flags);
    }

    if ( in_water )
    {
        medium_result = 2;
        friction = (definition->water_friction * radius_squared);
        buoyancy_scale = definition->runtime_water_buoyancy_scale;
        medium_mass_over_radius_cubed = global_water_mass_over_radius_cubed;
    }
    else
    {
        friction = (definition->air_friction * radius_squared);
        buoyancy_scale = definition->runtime_air_buoyancy_scale;
        medium_result = 1;
        medium_mass_over_radius_cubed = global_air_mass_over_radius_cubed;
    }

    mass = ((medium_mass_over_radius_cubed + mass_over_radius_cubed) * radius_cubed);
    if ( (definition->flags & (1u << _point_physics_no_gravity_bit)) != 0 )
        buoyancy_scale = 0.0f;

    if ( translational_force && mass != 0.0 )
    {
        float vy = translational_velocity->n[1];
        float vz = translational_velocity->n[2];
        translational_velocity->n[0] = (translational_force->n[0] * ((float)remaining / mass)) + translational_velocity->n[0];
        translational_velocity->n[1] = (translational_force->n[1] * ((float)remaining / mass)) + vy;
        translational_velocity->n[2] = (translational_force->n[2] * ((float)remaining / mass)) + vz;
    }

    translational_velocity->n[2] = ((((global_gravity * 30.0f) * 30.0f) * buoyancy_scale) * (float)remaining)
                                 + translational_velocity->n[2];

    /* fraction of velocity that relaxes toward the medium current this step */
    if ( mass != 0.0 )
    {
        damping = (((float)remaining / mass) * friction);
        if ( damping < 0.0 )
            damping = 0.0f;
        else if ( damping > 1.0 )
            damping = 1.0f;
    }
    else if ( friction != 0.0 )
    {
        damping = 1.0f;
    }
    else
    {
        damping = 0.0f;
    }

    {
        float vx = translational_velocity->n[0];
        float vy = translational_velocity->n[1];
        float dx = (current.n[0] - translational_velocity->n[0]);
        float dy = (current.n[1] - translational_velocity->n[1]);
        translational_velocity->n[2] = ((current.n[2] - translational_velocity->n[2]) * damping) + translational_velocity->n[2];
        translational_velocity->n[0] = (dx * damping) + vx;
        translational_velocity->n[1] = (dy * damping) + vy;
    }

    /* build collision query flags from the definition's collide-with bits (DB enum $72783576) */
    current_flags = definition->flags;
    if ( (definition->flags & (1u << _point_physics_water_collisions_bit)) == 0 || (flags & 4) != 0 )
        collision_flags = (1u << _collision_test_front_facing_surfaces_bit);
    else
        collision_flags = (1u << _collision_test_front_facing_surfaces_bit)
                        | (1u << _collision_test_media_bit);
    if ( (current_flags & (1u << _point_physics_structure_collisions_bit)) == 0 || (flags & 4) != 0 )
        collision_flags &= ~(1u << _collision_test_structure_bit);
    else
        collision_flags |= (1u << _collision_test_structure_bit);
    if ( (current_flags & (1u << _point_physics_flamethrower_collision_bit)) != 0 )
        /* clear objects + every per-type select (bits 7..19 = & 0xFFF0007F), then test objects|vehicles|scenery */
        collision_flags = (collision_flags & ~0xFFF80u)
                        | (1u << _collision_test_objects_bit)
                        | (1u << _collision_test_objects_vehicles_bit)
                        | (1u << _collision_test_objects_scenery_bit);

    for ( bounce = 0; bounce < 3; bounce = bounce + 1 )
    {
        float bounce_offset;

        displacement.n[0] = translational_velocity->n[0] * (float)remaining;
        displacement.n[1] = (translational_velocity->n[1] * (float)remaining);
        displacement.n[2] = (translational_velocity->n[2] * (float)remaining);

        if ( !collision_test_vector(collision_flags, position, &displacement, -1, &collision) )
        {
            if ( collision.location.leaf_index != -1 )
                *location = collision.location;
            position->n[0] = collision.point.n[0];
            position->n[1] = collision.point.n[1];
            position->n[2] = collision.point.n[2];
            return medium_result;
        }

        bounce_offset = radius <= 0.0049999999 ? radius : 0.0049999999f;

        if ( collision.type )
        {
            if ( collision.type == collision_result_structure )
                medium_result |= 4u;
        }
        else
        {
            medium_result |= 8u;
        }
        if ( collision.type == collision_result_object && (definition->flags & (1u << _point_physics_flamethrower_collision_bit)) != 0 )
            medium_result |= 4u;

        if ( collision_normal )
        {
            collision_normal->n[0] = collision.plane.n.n[0];
            collision_normal->n[1] = collision.plane.n.n[1];
            collision_normal->n[2] = collision.plane.n.n[2];
        }
        if ( collision_material_type )
            *collision_material_type = collision.material_type;

        component_vectors_from_normal3d(translational_velocity, &collision.plane.n, &parallel, &perpendicular);
        translational_velocity->n[0] = (((float)1.0 - definition->contact_friction) * perpendicular.n[0])
                                     - (parallel.n[0] * definition->elasticity);
        translational_velocity->n[1] = (((float)1.0 - definition->contact_friction) * perpendicular.n[1])
                                     - (parallel.n[1] * definition->elasticity);
        translational_velocity->n[2] = (((float)1.0 - definition->contact_friction) * perpendicular.n[2])
                                     - (parallel.n[2] * definition->elasticity);

        if ( collision.location.leaf_index != -1 )
            *location = collision.location;

        remaining = -((collision.t * (float)remaining) - (float)remaining);

        position->n[0] = (collision.plane.n.n[0] * bounce_offset) + collision.point.n[0];
        position->n[1] = ((collision.plane.n.n[1] * bounce_offset) + collision.point.n[1]);
        position->n[2] = ((collision.plane.n.n[2] * bounce_offset) + collision.point.n[2]);

        if ( remaining == 0.0 )
            return medium_result;
    }
    return medium_result;
}
