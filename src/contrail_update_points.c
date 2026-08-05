/* contrail_update_points @0x8373F740 — advance every point of a contrail's 4 point lists over dt.
 * Each point ages along its current rendering stage; when the stage timer elapses it advances to the
 * next sub-stage (re-rolling the per-stage rate from the definition's random ranges), or marks itself
 * dead when it runs past the last stage. Live points are physically integrated via point_physics_update.
 * After updating, dead tail points are unlinked/freed and a fully-dead single-point list is collapsed.
 *
 * Types: contrail_point_datum (flags bit0=just-spawned, bit1=accelerating, bit2=dead; state_index =
 * stage; time=age, delta=1/duration rate), contrail_datum (first_contrail_point_indices[4],
 * contrail_point_counts[4]), contrail_definition.states (contrail_point_state[], DB-verified). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/point_physics_definition.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/location.h"
#include "headers/contrail_datum.h"
#include "headers/contrail_point_datum.h"
#include "headers/contrail_point_datum_flags.h"
#include "headers/contrail_definition.h"
#include "headers/contrail_point_state.h"
#include "headers/contrail_point_state_scaleable_values_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern unsigned int point_physics_update(unsigned int flags, const point_physics_definition *definition, location *location, int16_t force_weather_palette_index, real_point3d *position, real_vector3d *translational_velocity, const real_vector3d *translational_force, real_vector3d *collision_normal, int16_t *collision_material_type, float radius, float dt);
extern void datum_delete(data_array *data, int index);

void contrail_update_points(uint16_t contrail_index, float dt)
{
    contrail_datum *contrail = DATA_ARRAY_ELEMENT(contrail_data, contrail_datum, contrail_index);
    contrail_definition *definition =
        TAG_GET(contrail_definition, contrail->definition_index);
    int list;
    int16_t *collision_material_type;   /* decompiler reads this output pointer uninitialized */
    _BYTE visited[144];                 /* indices of points visited this pass, in list order */

    for ( list = 0; list < 4; list = (int16_t)(list + 1) )
    {
        int16_t visited_count = 0;
        int point_index;
        contrail_point_datum *point = NULL;
        int visited_cursor;
        contrail_point_state *states = (contrail_point_state *)definition->states.address;

        for ( point_index = contrail->first_contrail_point_indices[list];
              point_index != -1;
              point_index = point->next_contrail_point_index )
        {
            point = DATA_ARRAY_ELEMENT(contrail_point_data, contrail_point_datum, point_index);

            if ( (point->flags & (1u << _contrail_point_living_dead_bit)) == 0 )      /* not dead: age along current stage */
            {
                point->time = (point->delta * dt) + point->time;
                while ( point->delta == 0.0 || point->time > 1.0 )
                {
                    int stage = point->state_index;
                    int next_stage = stage + 1;
                    if ( (point->flags & (1u << _contrail_point_transitioning_bit)) != 0 )      /* accelerating sub-stage: advance to next stage */
                    {
                        contrail_point_state *record = &states[next_stage];
                        int flags = record->scale_flags;
                        float base = record->duration_lower_bound;
                        float span;
                        float rate;

                        point->time = 0.0f;
                        point->state_index = next_stage;
                        if ( (flags & (1u << _contrail_state_duration_bit)) != 0 )
                            base = point->density * record->duration_lower_bound;   /* density-scaled */
                        span = record->duration_upper_bound - record->duration_lower_bound;
                        if ( (flags & (1u << _contrail_state_duration_delta_bit)) != 0 )
                            span = (record->duration_upper_bound - record->duration_lower_bound) * point->density;
                        rate = real_seed_random_range(get_global_local_random_seed_address(), 0.0f, span) + base;
                        point->delta = rate;
                        if ( rate != 0.0 )
                            point->delta = 1.0f / rate;
                        point->flags &= ~(1u << _contrail_point_transitioning_bit);
                    }
                    else                            /* decelerating sub-stage, or end of life */
                    {
                        contrail_point_state *record;
                        int flags;
                        float base;
                        float span;
                        float rate;

                        if ( next_stage >= definition->states.count )
                        {
                            point->flags |= (1u << _contrail_point_living_dead_bit);     /* past last stage: die */
                            break;
                        }
                        record = &states[stage];
                        flags = record->scale_flags;
                        point->time = 0.0f;
                        base = record->transition_duration_lower_bound;
                        if ( (flags & (1u << _contrail_state_transition_duration_bit)) != 0 )
                            base = point->density * record->transition_duration_lower_bound;
                        span = record->transition_duration_upper_bound - record->transition_duration_lower_bound;
                        if ( (flags & (1u << _contrail_state_transition_duration_delta_bit)) != 0 )
                            span = (record->transition_duration_upper_bound - record->transition_duration_lower_bound) * point->density;
                        rate = real_seed_random_range(get_global_local_random_seed_address(), 0.0f, span) + base;
                        point->delta = rate;
                        if ( rate != 0.0 )
                            point->delta = 1.0f / rate;
                        point->flags |= (1u << _contrail_point_transitioning_bit);
                    }
                }
            }

            if ( (point->flags & (1u << _contrail_point_new_bit)) != 0 )
            {
                point->flags &= ~(1u << _contrail_point_new_bit);            /* clear just-spawned; skip physics this frame */
            }
            else if ( (point->flags & (1u << _contrail_point_living_dead_bit)) == 0 )
            {
                contrail_point_state *record = &states[point->state_index];
                int physics_tag = record->physics.index;
                if ( physics_tag != -1 )
                    point_physics_update(
                        0,
                        TAG_GET(const point_physics_definition, physics_tag),
                        &point->location,
                        -1,
                        &point->position,
                        &point->velocity,
                        NULL,
                        NULL,
                        collision_material_type,
                        (record->width * 0.5f),
                        dt);
            }

            *(_DWORD *)&visited[4 * visited_count++] = point_index;
        }

        /* unlink/free dead points from the tail inward */
        visited_cursor = (uint16_t)visited_count;
        if ( visited_count > 1 )
        {
            do
            {
                int *entry;
                int this_index;
                int prev_index;
                contrail_point_datum *this_point;
                contrail_point_datum *prev_point;

                visited_cursor = (int16_t)(visited_cursor - 1);
                entry = (int *)&visited[4 * visited_cursor];
                this_index = *entry;
                prev_index = (uint16_t)*(entry - 1);
                this_point = DATA_ARRAY_ELEMENT(contrail_point_data, contrail_point_datum, *entry);
                prev_point = DATA_ARRAY_ELEMENT(contrail_point_data, contrail_point_datum, prev_index);
                if ( (this_point->flags & (1u << _contrail_point_living_dead_bit)) == 0 )
                    break;
                if ( (prev_point->flags & (1u << _contrail_point_living_dead_bit)) == 0 )
                    break;
                if ( this_point->next_contrail_point_index != -1 )
                    break;
                prev_point->next_contrail_point_index = -1;
                --contrail->contrail_point_counts[list];
                datum_delete(contrail_point_data, this_index);
            }
            while ( visited_cursor > 1 );
        }

        /* collapse a list down to nothing once its sole point has died */
        {
            if ( contrail->contrail_point_counts[list] == 1
              && (DATA_ARRAY_ELEMENT(contrail_point_data, contrail_point_datum,
                    contrail->first_contrail_point_indices[list])->flags & (1u << _contrail_point_living_dead_bit)) != 0 )
            {
                datum_delete(contrail_point_data, contrail->first_contrail_point_indices[list]);
                contrail->first_contrail_point_indices[list] = -1;
                contrail->contrail_point_counts[list] = 0;
            }
        }
    }
}
