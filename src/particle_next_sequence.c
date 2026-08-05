/* particle_next_sequence @0x8373DE10 — advance a particle to its next animation sequence, walking the
 * birth -> loop -> death sequence phases stored in its definition. Each phase picks a random sequence
 * from its sub-range; the particle's state byte tracks which phase it is in (0 birth, 1, 2 loop,
 * 3 death). When no sequence remains (or the bitmap has none) the particle dies. Returns nonzero
 * while alive. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/particle_datum.h"
#include "headers/particle_definition.h"
#include "headers/bitmap_group.h"
#include "headers/blam_data_globals.h"


extern uint32_t *get_global_local_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern void particle_die(int particle_index);

uint8_t particle_next_sequence(int particle_index)
{
    particle_datum *particle = DATUM_GET(particle_data, particle_datum, particle_index);
    int state = particle->state;
    particle_definition *definition = TAG_GET(particle_definition, particle->definition_index);
    bitmap_group *bitmap = TAG_GET(bitmap_group, definition->bitmap.index);
    int sequence;
    int bitmap_sequence_count;

    particle->sequence_index = -1;

    if ( !state )   /* birth phase */
    {
        int16_t birth_count = definition->initial_sequence_count;
        if ( birth_count > 0 )
            particle->sequence_index = definition->first_sequence_index
                                     + seed_random_range(get_global_local_random_seed_address(), 0, birth_count);
        ++particle->state;
    }

    if ( (uint16_t)particle->sequence_index == 0xFFFF && particle->state == 1 )
    {
        particle->state = 2;
    }
    else if ( particle->state != 2 )
    {
        goto check_death;
    }

    /* loop phase */
    if ( particle->life_time >= (double)particle->life_span
      || definition->looping_sequence_count <= 0 )
    {
        ++particle->state;
    }
    else
    {
        int16_t loop_count = definition->looping_sequence_count;
        particle->sequence_index = definition->initial_sequence_count + definition->first_sequence_index
                                 + seed_random_range(get_global_local_random_seed_address(), 0, loop_count);
    }

check_death:
    if ( (uint16_t)particle->sequence_index == 0xFFFF && particle->state == 3 )
    {
        int16_t death_count = definition->final_sequence_count;
        if ( death_count > 0 )
            particle->sequence_index = definition->looping_sequence_count
                                     + definition->initial_sequence_count
                                     + definition->first_sequence_index
                                     + seed_random_range(get_global_local_random_seed_address(), 0, death_count);
        ++particle->state;
    }

    sequence = particle->sequence_index;
    bitmap_sequence_count = bitmap->sequences.count;
    if ( sequence == -1 || bitmap_sequence_count == 0 )
    {
        particle_die(particle_index);
        return 0;
    }
    if ( sequence >= 0 )
    {
        int last_sequence = bitmap_sequence_count - 1;
        if ( sequence <= last_sequence )
            last_sequence = particle->sequence_index;
        particle->sequence_index = last_sequence;
        return 1;
    }
    particle->sequence_index = 0;
    return 1;
}
