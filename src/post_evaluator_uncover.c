/* post_evaluator_uncover @0x837EF4D0 — firing-position post-evaluator for the uncover behavior (move to
 * where the target can be engaged): with a target, positions with no line of sight cost +20, partial LOS
 * (1) costs +10, and full LOS positions are rejected outright unless they sit meaningfully closer than the
 * current distance to the target (inside (current - 7.5)²). A NULL position instead raises the running
 * post-evaluation bound by 20. Returns whether the position is still valid (1 for the NULL-position call). */

#include <stdint.h>
#include "headers/firing_position.h"
#include "headers/firing_position_evaluation_context.h"
#include "headers/ai_line_of_sight.h"

uint8_t post_evaluator_uncover(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position)
{
    if ( evaluation_context->has_target )
    {
        if ( !firing_position )
        {
            evaluation_context->post_evaluation_bound += 20.0f;
            return 1;
        }

        float cost = 0.0f;
        if ( !firing_position->line_of_sight )
        {
            cost = 20.0f;
        }
        else if ( firing_position->line_of_sight == _ai_line_of_sight_occluded )
        {
            cost = 10.0f;
        }
        else
        {
            float closer_distance = evaluation_context->target_current_distance - 7.5f;
            if ( closer_distance < 0.0f
              || firing_position->linear_distance_squared_to_target > closer_distance * closer_distance )
            {
                firing_position->rejected = 1;
                if ( !evaluation_context->allow_rejected_positions )
                    firing_position->valid = 0;
            }
        }
        firing_position->evaluation += cost;
    }

    return firing_position ? firing_position->valid : 1;
}
