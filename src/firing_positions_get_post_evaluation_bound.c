/* firing_positions_get_post_evaluation_bound @0x837EF7E0 — resets the evaluation context's post-evaluation
 * bound to zero, then runs the registered post-evaluation passes with no firing position (nullptr) so each
 * applicable pass can populate `post_evaluation_bound`/`post_evaluation_bounded` on the context itself.
 * Mirrors firing_position_post_evaluate.c's dispatch loop. */

#include <stdint.h>
#include "headers/post_evaluator_table_entry.h"

extern post_evaluator_table_entry global_post_evaluator_table[];

uint8_t firing_positions_get_post_evaluation_bound(int actor_index, firing_position_evaluation_context *evaluation_context)
{
    evaluation_context->post_evaluation_bound = 0.0f;

    uint8_t result = 1;
    post_evaluator_table_entry *entry = global_post_evaluator_table;
    do
    {
        if ( !entry->evaluation_function )
            break;
        if ( ((1 << evaluation_context->evaluation_mode) & entry->evaluation_mode_mask) != 0 )
            result = entry->evaluation_function(actor_index, evaluation_context, nullptr);
        ++entry;
    }
    while ( result );

    return result;
}
