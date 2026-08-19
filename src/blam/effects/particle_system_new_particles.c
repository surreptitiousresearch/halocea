/* particle_system_new_particles @0x83739CC0 — emit new particles for one emitter type this frame. The
 * number to spawn comes from the type's initial burst count (transient systems) or its accumulated
 * creation rate * dt (steady-state, with a fractional carry). First-person visibility flags can suppress
 * emission depending on whether the system is the local player's first-person weapon. Markers are
 * resolved from the host object (or the system position for unattached systems, or the first-person
 * weapon), and each spawned particle is initialized, placed via its creation function, and linked in.
 * (type flags: initial_count_scales burst-scale, nodraw_first_person/nodraw_third_person visibility) */

#include <stdint.h>
#include "headers/object_type.h"
#include "headers/data_array.h"
#include "headers/particle_system_definition.h"
#include "headers/particle_system_type_state.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/_object_definition.h"
#include "headers/object_attachment_definition.h"
#include "headers/data_iterator.h"
#include "headers/global_tag_instances.h"
#include "headers/particle_system_datum.h"
#include "headers/particle_system_flags.h"
#include "headers/particle_system_type_flags.h"
#include "headers/particle_system_type.h"
#include "headers/particle_system_type_create.h"
#include "headers/particle_type.h"
#include "headers/ps_particle_datum.h"
#include "headers/object_marker.h"
#include "headers/render_globals.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"


/* DEVIATION: the element signature was re-declared here with `int type_index` and a `const`
 * marker; the handler definition TUs (particle_system_new_particle_default/_explosion/_jet) and
 * the DB applied type at 0x8211D4F4 both spell it
 * `(const particle_system_datum *, int16_t, ps_particle_datum *, object_marker *)`. */
extern void (*particle_creation_functions[])(const particle_system_datum *system, int16_t type_index,
        ps_particle_datum *particle, object_marker *marker);

extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern uint8_t local_player_is_first_person(int16_t local_player_index);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern location *object_get_location(int object_index, location *location_out);
extern int first_person_weapon_get_weapon_index(int16_t local_player_index);
extern int16_t first_person_weapon_get_marker_by_name(int weapon_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern int datum_new(data_array *data);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern void scenario_location_from_point(location *location, const real_point3d *point);
extern void datum_delete(data_array *data, int index);

void particle_system_new_particles(particle_system_datum *system, int16_t type_index, float dt)
{
    unsigned int is_burst = (system->flags >> _particle_system_initializing_bit) & 1;
    particle_type *type = &system->types[type_index];
    particle_system_type *type_record =
        (particle_system_type *)(TAG_GET(particle_system_definition, system->definition_index))->types.address
        + type_index;
    particle_system_type_state *state_record = is_burst ? nullptr
        : (particle_system_type_state *)type_record->type_states.address + type->state_index;
    int object_index = system->object_index;
    int16_t first_person_role = 0;     /* 0 = not the player's weapon, 1 = third-person owner, -1 = first */
    int spawn_count;
    int target_count;
    data_iterator iterator;
    player_datum *player;
    object_marker markers[8];
    int16_t marker_count;

    /* find whether this system belongs to a local player's held weapon */
    data_iterator_new(&iterator, player_data);
    player = data_iterator_next(&iterator);
    if ( player )
    {
        while ( !object_try_and_get_and_verify_type(player->unit_index, object_mask_unit)
             || object_index != unit_inventory_get_weapon(player->unit_index,
                    ((unit_datum *)DATUM_GET(object_header_data, object_header_datum,
                        player->unit_index)->datum)->unit.current_weapon_index) )
        {
            player = data_iterator_next(&iterator);
            if ( !player )
                goto role_resolved;
        }
        first_person_role = ((uint16_t)player->local_player_index == 0xFFFF) ? -1 : 1;
    }

role_resolved:
    /* first-person visibility gate: emission proceeds when the system is not the player's weapon, or
     * when both first-person visibility conditions hold; otherwise it is suppressed. */
    if ( first_person_role
      && !(((type_record->flags & (1u << _particle_system_type_nodraw_third_person_bit)) == 0 || (first_person_role != -1 && local_player_is_first_person(render.local_player_index)))
        && ((type_record->flags & (1u << _particle_system_type_nodraw_first_person_bit)) == 0 || first_person_role != 1 || !local_player_is_first_person(render.local_player_index))) )
        return;

    /* how many particles to spawn this frame */
    if ( is_burst )
    {
        spawn_count = type_record->initial_particle_count;
        if ( (type_record->flags & (1u << _particle_system_type_initial_count_scales_bit)) != 0 )
            spawn_count = (int)(((float)spawn_count * system->scale) + 0.5f);
    }
    else
    {
        float rate = (type->variables.particle_creation_rate * dt);
        float fractional = type->fractional_particle_count;
        int particle_count = (uint16_t)type->particle_count;
        int whole = (int)rate;
        spawn_count = particle_count + whole;
        type->fractional_particle_count = ((rate - (float)whole) + fractional);
        if ( type->fractional_particle_count > 1.0 )
        {
            type->fractional_particle_count = type->fractional_particle_count - 1.0f;
            ++spawn_count;
        }
    }

    if ( render_particle_systems_enabled == 1 )
        spawn_count = (int)((float)spawn_count * 0.5f);

    target_count = spawn_count;

    if ( type->particle_count < spawn_count )
    {
        /* resolve emission markers */
        if ( system->object_index == -1 )
        {
            marker_count = 1;
            markers[0].matrix.n[3][0] = system->position.n[0];
            markers[0].matrix.n[3][1] = system->position.n[1];
            markers[0].matrix.n[3][2] = system->position.n[2];
            markers[0].matrix.n[0][0] = global_zero_vector3d->n[0];
            markers[0].matrix.n[0][1] = global_zero_vector3d->n[1];
            markers[0].matrix.n[0][2] = global_zero_vector3d->n[2];
        }
        else
        {
            object_datum *object = DATUM_GET(object_header_data, object_header_datum,
                                       system->object_index)->datum;
            const char *marker_name =
                ((object_attachment_definition *)(TAG_GET(_object_definition, object->definition_index))
                     ->attachments.address)[system->attachment_index].marker_name;
            marker_count = object_get_marker_by_name(system->object_index, marker_name, markers, 8);
            object_get_location(system->object_index, &system->location);
            if ( !marker_count )
            {
                int weapon_index = first_person_weapon_get_weapon_index(render.local_player_index);
                if ( weapon_index != -1 )
                {
                    marker_count = first_person_weapon_get_marker_by_name(weapon_index, marker_name, markers, 8);
                    object_get_location(weapon_index, &system->location);
                }
            }
        }

        if ( (uint16_t)system->location.cluster_index != 0xFFFF )
        {
            int16_t spawned = 0;
            while ( type->particle_count < target_count )
            {
                int new_index;
                ps_particle_datum *particle;
                int16_t creation_function;

                if ( !marker_count || spawned >= 128 )
                    break;
                new_index = datum_new(system_particles);
                if ( new_index == -1 )
                    break;

                /* recovered: (char*)data + ((new_index<<7)&0x7FFF80) -> typed element at datum absolute index (stride 128 == sizeof(ps_particle_datum), unchecked) */
                particle = DATA_ARRAY_ELEMENT(system_particles, ps_particle_datum, new_index);
                creation_function = is_burst ? type_record->initial_particle_creation_physics
                                             : state_record->particle_creation_physics;
                particle->sprite_index = -1.0f;
                particle->valid = 1;
                particle->state_index = -1;
                particle->transition_state_index = -1;
                particle->states_moving_forward = 1;
                particle->rotation = real_seed_random_range(get_global_local_random_seed_address(), 0.0f, TWO_PI);

                {
                    int16_t marker = seed_random_range(get_global_local_random_seed_address(), 0, marker_count);
                    /* creation_function's domain is enum particle_system_type_create
                     * (headers/particle_system_type_create.h) — the index arrives from tag
                     * data, never a literal */
                    particle_creation_functions[creation_function](system, type_index, particle, &markers[marker]);
                }
                scenario_location_from_point(&particle->location, &particle->position);
                if ( (uint16_t)particle->location.cluster_index == 0xFFFF )
                {
                    datum_delete(system_particles, new_index);
                }
                else
                {
                    particle->next_particle_index = type->first_particle_index;
                    type->particle_count = (int16_t)(type->particle_count + 1);
                    type->first_particle_index = new_index;
                }
                spawned = spawned + 1;
            }
        }
    }

    if ( (float)type->particle_count < type->variables.minimum_particle_count )
        type->time_left_in_state = type->time_left_in_state * 0.30000001f;
}
