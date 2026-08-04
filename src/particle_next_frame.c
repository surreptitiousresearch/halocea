/* particle_next_frame @0x8373E018 — step a particle one animation frame. Resets the frame timer and
 * advances the frame index forward, or backward when the particle plays in reverse (flag 0x1), rolling
 * to a new sequence (particle_next_sequence) at the ends. Returns nonzero while the particle is alive.
 * (frame count = sequence's sprites.count) */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/particle_datum.h"
#include "headers/particle_definition.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/particle_flags.h"
#include "headers/blam_data_globals.h"


extern uint8_t particle_next_sequence(int particle_index);

uint8_t particle_next_frame(int particle_index)
{
    particle_datum *particle = DATA_ARRAY_ELEMENT(particle_data, particle_datum, particle_index);
    int reverse = particle->flags & (1u << _particle_animates_backwards_bit);
    bitmap_group *bitmap = TAG_GET(bitmap_group, TAG_GET(particle_definition, particle->definition_index)->bitmap.index);
    bitmap_group_sequence *sequences = (bitmap_group_sequence *)bitmap->sequences.address;
    uint8_t result;

    particle->frame_time = 0.0f;

    if ( reverse )
    {
        int frame_index = particle->frame_index;
        if ( frame_index <= 0 )
        {
            result = particle_next_sequence(particle_index);
            if ( result )
                particle->frame_index = sequences[particle->sequence_index].sprites.count - 1;
        }
        else
        {
            particle->frame_index = frame_index - 1;
            return 1;
        }
    }
    else
    {
        int next_frame = particle->frame_index + 1;
        if ( next_frame >= sequences[particle->sequence_index].sprites.count )
        {
            result = particle_next_sequence(particle_index);
            next_frame = 0;
        }
        else
        {
            result = 1;
        }
        particle->frame_index = next_frame;
    }
    return result;
}
