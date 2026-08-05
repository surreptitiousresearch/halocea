/* particle_update_physics @0x8373E368 — integrate one particle's motion for dt. Settled particles
 * (flag 0x2) just verify their host object still exists. Object-attached particles decay their velocity
 * by the physics air friction and integrate locally. Free particles run the full point_physics_update,
 * spawning collision particle/material effects and dying/settling per the definition's collision flags.
 * Finally the particle's facing is aligned to its velocity and its spin advanced.
 *
 * (definition flags: _particle_dies_at_rest_bit 0x10, _particle_dies_on_contact_with_structure_bit 0x20,
 * _particle_dies_on_contact_with_water_bit 0x80, _particle_dies_on_contact_with_air_bit 0x100.
 * Particle radius scale = (life_time/life_span)*(final-initial radius modifier)+initial, times the
 * particle radius.) */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/particle_datum.h"
#include "headers/particle_definition.h"
#include "headers/point_physics_definition.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/location.h"
#include "headers/particle_definition_flags.h"
#include "headers/particle_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"
#include "headers/real_point3d.h"
#include "headers/real_point3d.h"
#include "headers/real_point3d.h"
extern float __fsqrts(float);


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern unsigned int point_physics_update(unsigned int flags, const point_physics_definition *definition, location *location, int16_t force_weather_palette_index, real_point3d *position, real_vector3d *translational_velocity, const real_vector3d *translational_force, real_vector3d *collision_normal, int16_t *collision_material_type, float radius, float dt);
extern float point_physics_definition_get_mass(const point_physics_definition *physics, float radius);
extern void particle_effect_new(particle_datum *particle, unsigned int group_tag, int effect_index, float scale);
extern uint8_t material_effect_visible(const real_point3d *origin);
extern void material_effect_new(int effects_definition_index, int16_t effect_index, int16_t material_index, const real_point3d *origin, const real_vector3d *normal, const location *location, float scale, uint8_t is_player);
extern void particle_die(int particle_index);
extern void datum_delete(data_array *data, int index);
extern void *datum_get(data_array *array, int index);

int particle_update_physics(int particle_index, float dt)
{
    particle_datum *particle = datum_get(particle_data, particle_index);
    uint16_t flags = particle->flags;
    int object_index = particle->object_index;
    particle_definition *definition = TAG_GET(particle_definition, particle->definition_index);
    point_physics_definition *physics;
    char velocity_aligned = 0;
    float *velocity;

    float radius_lo = definition->initial_radius_modifier;
    float radius_hi = definition->final_radius_modifier;

    if ( (flags & (1u << _particle_at_rest_bit)) != 0 )
    {
        if ( object_index == -1 || object_try_and_get_and_verify_type(object_index, object_mask_all) )
            return 1;
        datum_delete(particle_data, particle_index);
        return 0;
    }

    physics = TAG_GET(point_physics_definition, definition->physics.index);

    if ( object_index != -1 )
    {
        /* object-attached: decay velocity by friction and integrate locally */
        particle_definition *p_definition;
        float radius_value;
        double friction_force;
        double mass;
        double decay;

        if ( (flags & (1u << _particle_attached_to_local_player_bit)) == 0 && !object_try_and_get_and_verify_type(object_index, object_mask_all) )
        {
            datum_delete(particle_data, particle_index);
            return 0;
        }

        /* recovered: *((_DWORD *)p + 1) -> particle->definition_index */
        p_definition = TAG_GET(particle_definition, particle->definition_index);
        /* recovered: *((float *)p + 5/6/23) -> particle->life_time / life_span / radius */
        radius_value = (((particle->life_time / particle->life_span)
                                     * (p_definition->final_radius_modifier - p_definition->initial_radius_modifier))
                             + p_definition->initial_radius_modifier)
                     * particle->radius;
        friction_force = (physics->air_friction * radius_value) * radius_value;
        mass = point_physics_definition_get_mass(physics, radius_value);

        if ( mass == 0.0 )
        {
            if ( friction_force == 0.0 )
                decay = 1.0;
            else
                decay = 0.0;
        }
        else
        {
            decay = -((((float)friction_force / (float)mass) * dt) - 1.0f);
            if ( decay < 0.0 )
                decay = 0.0;
            else if ( decay > 1.0 )
                decay = 1.0;
        }

        velocity = particle->translational_velocity.n;
        velocity_aligned = 1;
        {
            float vx = (float)decay * particle->translational_velocity.n[0];
            float vy = (particle->translational_velocity.n[1] * (float)decay);
            float vz = (particle->translational_velocity.n[2] * (float)decay);
            float px = particle->position.n[0];
            float py = particle->position.n[1];
            float pz = particle->position.n[2];
            particle->translational_velocity.n[0] = vx;
            particle->translational_velocity.n[1] = vy;
            particle->translational_velocity.n[2] = vz;
            particle->position.n[1] = (vy * dt) + py;
            particle->position.n[2] = (vz * dt) + pz;
            particle->position.n[0] = (vx * dt) + px;
        }
    }
    else
    {
        /* free particle: full physics step with collision response */
        /* recovered: p[23] / p[5] / p[6] / *((_DWORD *)p + 1) -> particle->radius / life_time / life_span / definition_index */
        float radius = particle->radius;
        float life_fraction = (particle->life_time / particle->life_span);
        particle_definition *p_definition = TAG_GET(particle_definition, particle->definition_index);
        int16_t collision_material_type;
        real_vector3d collision_normal;
        unsigned int collision;
        int collided_solid;

        velocity = particle->translational_velocity.n;
        collision = point_physics_update(
                0,
                physics,
                &particle->location,
                -1,
                &particle->position,
                &particle->translational_velocity,
                NULL,
                &collision_normal,
                &collision_material_type,
                (((life_fraction * (p_definition->final_radius_modifier - p_definition->initial_radius_modifier))
                              + p_definition->initial_radius_modifier) * radius),
                dt);

        collided_solid = collision & 4;
        if ( (collision & 4) != 0 )
        {
            int collision_effect = definition->collision_effect.index;
            if ( collision_effect != -1 || definition->collision_material_effects.index )
            {
                float speed = (__fsqrts(((velocity[2] * velocity[2])
                                            + ((*velocity * *velocity) + (velocity[1] * velocity[1])))) - 0.5f);
                if ( speed < 0.0 )
                    speed = 0.0f;
                else if ( speed > 1.0 )
                    speed = 1.0f;

                if ( collision_effect != -1 )
                    particle_effect_new(particle, definition->collision_effect.group_tag, collision_effect, speed);
                if ( definition->collision_material_effects.index != -1 && material_effect_visible(&particle->position) )
                    material_effect_new(definition->collision_material_effects.index, 8, collision_material_type, &particle->position,
                            &collision_normal, &particle->location, speed, 0);
            }
            if ( (definition->flags & (1u << _particle_dies_on_contact_with_structure_bit)) != 0 )
            {
                if ( definition->collision_effect.index == -1 )
                {
                    particle_die(particle_index);
                    return 0;
                }
                datum_delete(particle_data, particle_index);
                return 0;
            }
        }

        if ( ((collision & 1) != 0 && (definition->flags & (1u << _particle_dies_on_contact_with_air_bit)) != 0)
          || ((collision & 2) != 0 && (definition->flags & (1u << _particle_dies_on_contact_with_water_bit)) != 0) )
        {
            particle_die(particle_index);
            return 0;
        }

        if ( collided_solid || (collision & 8) != 0 )
        {
            if ( collision_normal.n[2] > 0.80000001 )
                velocity_aligned = 1;
            particle->frame_span = definition->frames_per_second_contact_deterioration + particle->frame_span;
        }
    }

    /* align facing to velocity when moving fast enough, else handle settling */
    if ( ((*velocity * *velocity) + ((velocity[1] * velocity[1]) + (velocity[2] * velocity[2]))) >= 0.0625 )
    {
        particle->direction.n[0] = *velocity;
        particle->direction.n[1] = velocity[1];
        particle->direction.n[2] = velocity[2];
    }
    else if ( velocity_aligned )
    {
        if ( (definition->flags & (1u << _particle_dies_at_rest_bit)) != 0 )
        {
            particle_die(particle_index);
            return 0;
        }
        particle->rotation = (particle->angular_velocity * dt) + particle->rotation;
        particle->flags = particle->flags | (1u << _particle_at_rest_bit);   /* settle */
        return 1;
    }

    particle->rotation = (particle->angular_velocity * dt) + particle->rotation;
    return 1;
}
