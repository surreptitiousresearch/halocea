/* contrail_next_frame @0x8373F1C8 — advance a contrail to the next animation frame. The frame timer is
 * reset and the frame index incremented; if the current sequence/frame has run past the end of the
 * bitmap's sequence table, a new random sequence is rolled (from the definition's first-sequence +
 * sequence-count range) and the frame index reset to 0. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/contrail_datum.h"
#include "headers/contrail_definition.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"

extern uint32_t *get_global_local_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);

void contrail_next_frame(contrail_datum *contrail)
{
    __int16 sequence_index = contrail->sequence_index;
    __int16 next_frame = contrail->frame_index + 1;
    contrail_definition *definition = TAG_GET(contrail_definition, contrail->definition_index);
    bitmap_group *bitmap = TAG_GET(bitmap_group, definition->bitmap.index);

    contrail->frame_time = 0.0f;
    contrail->frame_index = next_frame;

    if ( sequence_index < 0
      || sequence_index >= bitmap->sequences.count
      || next_frame < 0
      || next_frame >= ((bitmap_group_sequence *)bitmap->sequences.address)[sequence_index].bitmap_count )
    {
        __int16 first_sequence = definition->first_sequence_index;
        __int16 last_sequence = definition->sequence_count + first_sequence;
        unsigned int *seed = get_global_local_random_seed_address();
        contrail->sequence_index = seed_random_range(seed, first_sequence, last_sequence);
        contrail->frame_index = 0;
    }
}
