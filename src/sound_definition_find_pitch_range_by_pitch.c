/* sound_definition_find_pitch_range_by_pitch @ 0x837F8AC0 — choose the pitch range whose
 * [bend_lower_bound, bend_upper_bound] band contains the requested pitch. If the supplied
 * current range still matches it is kept; otherwise all ranges (with permutations) are scanned,
 * returning an exact match or, failing that, the closest one by pitch ratio. Returns -1 if none. */

#include <stdint.h>
#include "headers/sound_definition.h"
#include "headers/sound_pitch_range.h"

int16_t sound_definition_find_pitch_range_by_pitch(const sound_definition *sound, float pitch, int16_t current_range)
{
    int result = -1;
    int count;
    int i;
    float best_ratio;

    /* keep the current range if it still matches and has permutations */
    if ( current_range != -1 && current_range < sound->pitch_ranges.count )
    {
        sound_pitch_range *range = (sound_pitch_range *)sound->pitch_ranges.address + current_range;
        if ( range->bend_lower_bound <= pitch && pitch <= range->bend_upper_bound && range->permutations.count )
            result = current_range;
    }

    if ( (int16_t)result != -1 )
        return result;

    count = sound->pitch_ranges.count;
    best_ratio = 3.4028235e38f;
    if ( count <= 0 )
        return result;

    for ( i = 0; i < count; i = (int16_t)(i + 1) )
    {
        sound_pitch_range *range = (sound_pitch_range *)sound->pitch_ranges.address + i;
        if ( range->permutations.count )
        {
            float ratio;
            if ( range->bend_lower_bound <= pitch && pitch <= range->bend_upper_bound )
                return i;
            if ( range->bend_upper_bound >= pitch )
                ratio = range->bend_lower_bound / pitch;
            else
                ratio = pitch / range->bend_upper_bound;
            if ( ratio < best_ratio )
            {
                result = i;
                best_ratio = ratio;
            }
        }
    }
    return result;
}
