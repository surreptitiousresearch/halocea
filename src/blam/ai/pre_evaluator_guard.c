/* pre_evaluator_guard @0x837EE9A8 — pre-evaluation bonus for the "guard" behavior: for every valid firing
 * position, add a bonus (0 to 8.0) to its running evaluation score — positions within the first half of
 * maximum_allowable_range get the full +8.0, the bonus tapers linearly to 0 at the range edge, and
 * positions at or beyond the range score nothing. */

#include <stdint.h>
#include "headers/firing_position.h"
#include "headers/firing_position_evaluation_context.h"

void pre_evaluator_guard(int actor_index, firing_position_evaluation_context *evaluation_context, int16_t firing_position_count, firing_position *firing_positions)
{
    for ( int16_t i = 0; i < firing_position_count; i++ )
    {
        firing_position *position = &firing_positions[i];
        if ( !position->valid )
            continue;

        float half_range = evaluation_context->maximum_allowable_range * 0.5f;
        float bonus;

        /* DEVIATION: earlier transcription had the outer arms swapped; binary gives the full bonus INSIDE
         * half range (fmr f12,f10 @0x837EEA00 on the not-taken dist>=half branch @0x837EE9FC) and zero at or
         * beyond max range (f12 keeps the 0.0 seeded by fmr f12,f9 @0x837EE9EC when bge @0x837EEA0C exits). */
        if ( position->path_distance_from_actor < half_range )
            bonus = 8.0f;
        else if ( position->path_distance_from_actor < evaluation_context->maximum_allowable_range )
            bonus = (1.0f / half_range) * (evaluation_context->maximum_allowable_range - position->path_distance_from_actor) * 8.0f;
        else
            bonus = 0.0f;

        position->evaluation += bonus;
    }
}
