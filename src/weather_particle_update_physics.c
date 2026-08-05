/* weather_particle_update_physics @0x8373CA78 — advance one weather particle's motion for a frame (sibling of
 * weather_particle_system_update / _new_particle). When the type has a non-zero speed range it re-randomizes the
 * particle's travel: it normalizes the current travel direction, picks a new speed (current magnitude plus a
 * random +/- variance, clamped to the type's [min,max]), blends the direction toward a fresh random direction by
 * the type's turbulence weight, scales it to the new speed, and integrates that into the particle's velocity.
 * It then runs the shared point_physics_update against the type's point-physics tag (using the water/air flag
 * from the system), applies a tiny deterministic per-particle positional drift, and wraps the particle back
 * inside the system's box.
 *
 * The system's per-type state (Hex-Rays' `&system->definition_index + 4*type_index`) resolves to
 * &system->types[type_index] (only box_width @+32 is used, via v6[8]). The weather tag definition's per-type
 * sub-block (stride 604) and the 84-byte particle are read at raw offsets (particle: position @+4, velocity @+16,
 * travel direction @+28, radius @+68). DEVIATION: point_physics_update's collision_material_type output is a
 * discarded scratch local (Hex-Rays showed it as the uninitialized pointer v27). */

#include <stdint.h>
#include "headers/weather_particle_system_globals.h"
#include "headers/weather_particle_datum.h"
#include "headers/weather_particle_type_definition.h"
#include "headers/weather_particle_system_definition.h"
#include "headers/point_physics_definition.h"
#include "headers/location.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);


extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);
extern unsigned int point_physics_update(unsigned int flags, const point_physics_definition *definition, location *location, int16_t force_weather_palette_index, real_point3d *position, real_vector3d *translational_velocity, const real_vector3d *translational_force, real_vector3d *collision_normal, int16_t *collision_material_type, float radius, float dt);
extern void weather_particle_system_wrap_point(float box_width, const real_point3d *point, real_point3d *result);

void weather_particle_update_physics(int16_t system_index, int16_t type_index, uint16_t particle_index)
{
    weather_particle_system_datum *system = &weather_particle_system_globals.systems[system_index];
    float box_width = system->types[type_index].box_width;

    weather_particle_system_definition *definition = TAG_GET(weather_particle_system_definition, system->definition_index);
    weather_particle_type_definition *type_def = (weather_particle_type_definition *)definition->particle_types.address + type_index;
    weather_particle_datum *particle = DATA_ARRAY_ELEMENT(weather_particle_data, weather_particle_datum, particle_index);

    if ( type_def->acceleration_lower_bound != 0.0f || type_def->acceleration_upper_bound != 0.0f )
    {
        real_vector3d *direction = &particle->acceleration;
        float speed = __fsqrts(direction->n[2] * direction->n[2]
                + (direction->n[0] * direction->n[0] + direction->n[1] * direction->n[1]));
        if ( __fabs(speed) < 0.000099999997f )
        {
            speed = 0.0f;
        }
        else
        {
            direction->n[0] *= 1.0f / speed;
            direction->n[1] *= 1.0f / speed;
            direction->n[2] *= 1.0f / speed;
        }

        float variance = type_def->acceleration_magnitude_rate;
        float blend = 1.0f - type_def->acceleration_turning_rate;
        float new_speed =
                real_seed_random_range(get_global_local_random_seed_address(), -variance, variance) + speed;
        if ( new_speed < type_def->acceleration_lower_bound )
            new_speed = type_def->acceleration_lower_bound;
        else if ( new_speed > type_def->acceleration_upper_bound )
            new_speed = type_def->acceleration_upper_bound;

        real_vector3d turbulence;
        seed_random_direction3d(get_global_local_random_seed_address(), &turbulence);
        float turb = type_def->acceleration_turning_rate;
        direction->n[0] = turb * turbulence.n[0] + direction->n[0] * blend;
        direction->n[1] = direction->n[1] * blend + turb * turbulence.n[1];
        direction->n[2] = turb * turbulence.n[2] + direction->n[2] * blend;
        direction->n[0] *= new_speed;
        direction->n[1] *= new_speed;
        direction->n[2] *= new_speed;

        float dt = system->time_delta_sec;
        real_vector3d *velocity = &particle->velocity;
        velocity->n[0] += direction->n[0] * dt;
        velocity->n[1] += direction->n[1] * dt;
        velocity->n[2] += direction->n[2] * dt;
    }

    point_physics_definition *physics = TAG_GET(point_physics_definition, type_def->physics.index);
    int16_t collision_material_type;
    point_physics_update(system->under_water == 0 ? 5 : 7, physics, &system->location,
            system->weather_palette_index, &particle->position,
            &particle->velocity, nullptr, nullptr, &collision_material_type,
            particle->radius, system->time_delta_sec);

    unsigned int particle_seed = particle_index;
    real_vector3d drift_storage;
    real_point3d *drift = (real_point3d *)seed_random_direction3d(&particle_seed, &drift_storage);
    particle->position.n[0] += drift->n[0] * 0.001f;
    particle->position.n[1] += drift->n[1] * 0.001f;
    particle->position.n[2] += drift->n[2] * 0.001f;
    weather_particle_system_wrap_point(box_width, drift, &particle->position);
}
