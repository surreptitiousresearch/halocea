/* post_evaluator_attack @0x837EF5A0 — "attack" post-evaluator for firing-position selection: if the
 * context has no target, accepts trivially (the position's own validity, or 1 if just bounding the
 * context). Otherwise scores by line-of-sight quality: full LOS gets the smallest bonus (2.5/5.0
 * depending on whether LOS is merely optional), blocked LOS with LOS required rejects the position
 * (invalidating it unless rejected positions are allowed), and no-LOS-info gets the largest bonus/bound
 * (6.0 if optional, 15.0 if required) — when scoring a specific position, added to its running evaluation
 * score; when just bounding the context (no position given), applied to the context's own bound. */

#include <stdint.h>
#include "headers/firing_position_evaluation_context.h"
#include "headers/firing_position.h"
#include "headers/ai_line_of_sight.h"

uint8_t post_evaluator_attack(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position)
{
    if ( !evaluation_context->has_target )
        return firing_position ? firing_position->valid : 1;

    if ( firing_position )
    {
        float bonus = 0.0f;

        if ( firing_position->line_of_sight )
        {
            if ( firing_position->line_of_sight == _ai_line_of_sight_occluded )
            {
                bonus = evaluation_context->target_line_of_sight_optional ? 2.5f : 5.0f;
            }
            else if ( !evaluation_context->target_line_of_sight_optional )
            {
                firing_position->rejected = 1;
                if ( !evaluation_context->allow_rejected_positions )
                    firing_position->valid = 0;
            }
        }
        else if ( evaluation_context->target_line_of_sight_optional )
        {
            bonus = 6.0f;
        }
        else
        {
            bonus = 15.0f;
        }

        firing_position->evaluation = firing_position->evaluation + bonus;
        return firing_position->valid;
    }

    if ( evaluation_context->target_line_of_sight_optional )
        evaluation_context->post_evaluation_bound = evaluation_context->post_evaluation_bound + 6.0f;
    else
        evaluation_context->post_evaluation_bound = evaluation_context->post_evaluation_bound + 15.0f;

    return 1;
}
