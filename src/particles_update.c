/* particles_update @0x8373ED78 — advances every live particle this frame. Particles untouched for more
 * than 15 render frames are reaped; otherwise each particle's age advances by dt and, once it exceeds
 * its lifespan (unless it has zero lifespan or its definition has final sequences to play), the
 * particle dies. Living particles run their per-frame timing and physics passes. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/render_globals.h"
#include "headers/particle_datum.h"
#include "headers/particle_definition.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void datum_delete(data_array *data, int index);
extern uint8_t particle_update_frame_time(int particle_index, float dt);
extern int particle_update_physics(int particle_index, float dt);
extern void particle_die(int particle_index);

/* dt is single-precision: caller game_frame builds f1 via fmuls (0x83687E00) and the age add
 * (fadds f0,f0,f31 @0x8373EE18) consumes it with no frsp. Hex-Rays widened it to double. */
void particles_update(float dt)
{
    int i;

    for ( i = data_next_index(particle_data, -1); i != -1; i = data_next_index(particle_data, i) )
    {
        particle_datum *particle = DATUM_GET(particle_data, particle_datum, i);
        particle_definition *definition = TAG_GET(particle_definition, particle->definition_index);
        int had_zero_age = particle->life_time == 0.0f;

        if ( render.frame_index - particle->last_rendered_frame_index > 15 )
        {
            datum_delete(particle_data, i);
            continue;
        }

        {
            float new_age   = particle->life_time + dt;
            float lifespan  = particle->life_span;
            particle->life_time = new_age;
            if ( new_age < lifespan || had_zero_age || definition->final_sequence_count )
            {
                if ( particle_update_frame_time(i, dt) )
                    particle_update_physics(i, dt);
            }
            else
            {
                particle_die(i);
            }
        }
    }
}
