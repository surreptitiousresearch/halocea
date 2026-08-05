#include <stdint.h>
#include "headers/sound_pitch_range.h"

void try_to_reset_permutations(sound_pitch_range *range)
{
    int16_t actual_permutation_count = range->actual_permutation_count;

    if ((((1 << actual_permutation_count) - 1) & ~range->runtime_permutation_flags) == 0)
    {
        range->runtime_permutation_flags = 0;
        if (actual_permutation_count > 1)
            range->runtime_permutation_flags = 1 << range->runtime_last_permutation_index;
    }
}
