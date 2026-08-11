/* sound_definition_promote @0x83715520 — per-tick "promotion" bookkeeping for a sound tag's playback-limit
 * throttle: accumulates elapsed render time into a decaying counter (runtime_maximum_play_time rate,
 * runtime_promotion_counter accumulator, runtime_promotion_time snapshot) and, if the accumulator exceeds
 * rate*promotion_count, returns a sound_promotion_result: when no promotion (replacement) sound is present
 * (promotion_sound.index == -1) it returns _sound_promotion_dont_play, otherwise _sound_promotion_do
 * (zeroing the accumulator). Returns _sound_promotion_dont if promotion_count is zero or the accumulator
 * is under threshold.
 *
 * DEVIATION: the decompiler rendered the accumulator's negative-clamp as `__CFADD__(v8, 0x80000000) ? 0 : v8`
 * (carry-flag-from-adding-0x80000000 trick) — this is exactly "clamp to zero if negative", restored as such;
 * same sign-trick tautology class as convex_hull2d_test_point_indexed.c. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/sound_manager_globals.h"
#include "headers/sound_definition.h"
#include "headers/sound_promotion_result.h"

int16_t sound_definition_promote(int definition_index)
{
    sound_definition *definition = TAG_GET(sound_definition, definition_index);

    int16_t limit = definition->promotion_count;
    if ( !limit )
        return _sound_promotion_dont;

    int rate = definition->runtime_maximum_play_time;
    int threshold = rate * limit;

    int accumulator = definition->runtime_promotion_time - sound_manager_globals.render_time
                    + definition->runtime_promotion_counter;
    definition->runtime_promotion_counter = accumulator;

    int clamped = (accumulator < 0) ? 0 : accumulator;
    definition->runtime_promotion_counter = clamped;
    definition->runtime_promotion_time = sound_manager_globals.render_time;
    definition->runtime_promotion_counter = rate + clamped;

    if ( rate + clamped > threshold )
    {
        if ( definition->promotion_sound.index == -1 )
        {
            definition->runtime_promotion_counter = clamped;
            return _sound_promotion_dont_play;
        }
        else
        {
            definition->runtime_promotion_counter = 0;
            return _sound_promotion_do;
        }
    }

    return _sound_promotion_dont;
}
