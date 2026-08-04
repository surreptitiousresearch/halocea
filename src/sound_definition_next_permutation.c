/* sound_definition_next_permutation @ 0x837F8C10 — pick the next permutation to play within a
 * pitch range. A forced "next permutation" override (set when looping) wins; otherwise a random
 * permutation is chosen, using a per-range played-bitmask so all permutations are heard before
 * any repeats, and weighted by each permutation's skip fraction. For looping sounds without an
 * override, returns -1. Returns the chosen permutation index. */

#include <stdint.h>
#include "headers/sound_definition.h"
#include "headers/sound_definition_flags.h"
#include "headers/sound_pitch_range.h"
#include "headers/sound_permutation.h"

extern uint32_t *get_global_local_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern float real_seed_random(uint32_t *seed);

extern void try_to_reset_permutations(sound_pitch_range *range);
int16_t sound_definition_next_permutation(sound_definition *sound, int16_t pitch_range_index,
                                      int16_t looping_last_permutation_index)
{
    sound_pitch_range *range = (sound_pitch_range *)sound->pitch_ranges.address + pitch_range_index;
    int forced = (uint16_t)range->runtime_discarded_permutation_index;
    int16_t attempts = 0;
    int16_t count;
    int candidate;

    if ( forced != 0xFFFF )
    {
        range->runtime_last_permutation_index = forced;
        range->runtime_discarded_permutation_index = -1;
        return forced;
    }

    if ( (sound->flags & (1u << _sound_definition_linked_permutations_bit)) == 0 || looping_last_permutation_index == -1 )
    {
        count = range->actual_permutation_count;
        candidate = seed_random_range(get_global_local_random_seed_address(), 0, count);
        while ( 1 )
        {
            /* DEVIATION: collapsed inlined copy of try_to_reset_permutations@0x837F8BC8 (zero-xref donor) back into a call; local `n` was only used within this block and is now unnecessary. */
            try_to_reset_permutations(range);

            {
                int played = range->runtime_permutation_flags;
                if ( ((1 << candidate) & played) == 0 )
                {
                    range->runtime_permutation_flags = (1 << candidate) | played;
                    if ( attempts++ == 16 )
                        break;
                    /* accept this permutation unless its skip fraction rejects it */
                    if ( real_seed_random(get_global_local_random_seed_address())
                         >= (double)((sound_permutation *)range->permutations.address)[candidate].skip_fraction )
                        break;
                }
            }
            candidate = (int16_t)(candidate + 1);
            if ( candidate == range->actual_permutation_count )
                candidate = 0;
        }
        range->runtime_last_permutation_index = candidate;
        return candidate;
    }
    return -1;
}
