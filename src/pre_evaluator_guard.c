/* pre_evaluator_guard @0x837EE9B0 — pre-evaluation bonus for the "guard" behavior: for every valid firing
 * position, add a bonus (0 to 8.0, scaled by how close path_distance_from_actor is to
 * maximum_allowable_range) to its running evaluation score — positions near the edge of the allowed range
 * score higher, up to the full +8.0 once at or beyond it; positions within the first half of the range score
 * nothing. */

#include <stdint.h>
#include "headers/firing_position.h"
#include "headers/firing_position_evaluation_context.h"

void pre_evaluator_guard(int actor_index, firing_position_evaluation_context *evaluation_context, int16_t firing_position_count, firing_position *firing_positions)
{
    for ( __int16 i = 0; i < firing_position_count; i++ )
    {
        firing_position *position = &firing_positions[i];
        if ( !position->valid )
            continue;

        float half_range = evaluation_context->maximum_allowable_range * 0.5f;
        float bonus = 0.0f;

        if ( position->path_distance_from_actor >= half_range )
        {
            if ( position->path_distance_from_actor < evaluation_context->maximum_allowable_range )
                bonus = (1.0f / half_range) * (evaluation_context->maximum_allowable_range - position->path_distance_from_actor) * 8.0f;
            else
                bonus = 8.0f;
        }

        position->evaluation += bonus;
    }
}
