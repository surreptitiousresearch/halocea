/* post_evaluator_hide @0x837EF3F8 — firing-position post-evaluator for the hide behavior: with a target,
 * fully exposed positions (no LOS is what hiding wants) score nothing; partial LOS grades cost 4/10/12 by
 * class (3 / other / 2); positions with clear LOS (1) are rejected — unless the actor is being forced to
 * flee, where they merely cost 6. A NULL position raises the running post-evaluation bound by 12. */

#include <stdint.h>
#include "headers/firing_position.h"
#include "headers/firing_position_evaluation_context.h"
#include "headers/ai_line_of_sight.h"

uint8_t post_evaluator_hide(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position)
{
    if ( evaluation_context->has_target )
    {
        if ( !firing_position )
        {
            evaluation_context->post_evaluation_bound += 12.0f;
            return 1;
        }

        unsigned int line_of_sight = (uint16_t)firing_position->line_of_sight;
        float cost = 0.0f;
        if ( line_of_sight > _ai_line_of_sight_obstructed )
        {
            /* out-of-range grade: no cost */
        }
        else if ( !line_of_sight )
        {
            goto reject;
        }
        else if ( line_of_sight == _ai_line_of_sight_occluded )
        {
            if ( evaluation_context->evaluation_data.___u0.panic.forced_to_flee )
            {
                cost = 6.0f;
            }
            else
            {
reject:
                firing_position->rejected = 1;
                if ( !evaluation_context->allow_rejected_positions )
                    firing_position->valid = 0;
                goto accumulate;
            }
        }
        else if ( line_of_sight == _ai_line_of_sight_from_cover )
        {
            cost = 12.0f;
        }
        else if ( line_of_sight == _ai_line_of_sight_to_cover )
        {
            cost = 4.0f;
        }
        else
        {
            cost = 10.0f;
        }
accumulate:
        firing_position->evaluation += cost;
    }

    return firing_position ? firing_position->valid : 1;
}
