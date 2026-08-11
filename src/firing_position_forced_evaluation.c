/* firing_position_forced_evaluation @0x837EF8E8 — evaluates a single firing position "from scratch": resets
 * its valid/rejected/evaluation state, runs pre-evaluation for it alone, computes line-of-sight when the
 * context has a target, then post-evaluates. Returns the resulting validity. */

#include <stdint.h>
#include "headers/firing_position.h"
#include "headers/firing_position_evaluation_context.h"

extern void firing_position_pre_evaluate(int actor_index, firing_position_evaluation_context *evaluation_context, int firing_position_count, firing_position *firing_positions);
extern void firing_position_compute_line_of_sight(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position);
extern uint8_t firing_position_post_evaluate(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position);

uint8_t firing_position_forced_evaluation(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position)
{
    firing_position->valid = 1;
    firing_position->evaluation = 0.0f;
    firing_position->rejected = 0;
    firing_position->pre_evaluation = 0.0f;

    firing_position_pre_evaluate(actor_index, evaluation_context, 1, firing_position);

    if ( firing_position->valid )
    {
        if ( evaluation_context->has_target )
            firing_position_compute_line_of_sight(actor_index, evaluation_context, firing_position);

        firing_position->pre_evaluation = firing_position->evaluation;
        firing_position->valid = firing_position_post_evaluate(actor_index, evaluation_context, firing_position);
    }

    return firing_position->valid;
}
