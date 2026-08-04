/* firing_position_post_evaluate @0x837EF730 — run the registered post-evaluation passes for a single firing
 * position. Each table entry whose mode mask includes the context's evaluation mode may veto the position;
 * evaluation stops as soon as a pass returns false. Returns whether the position survived all passes. */

#include <stdint.h>
#include "headers/post_evaluator_table_entry.h"

extern post_evaluator_table_entry global_post_evaluator_table[];

uint8_t firing_position_post_evaluate(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position)
{
    int result = 1;
    post_evaluator_table_entry *entry = global_post_evaluator_table;
    do
    {
        if ( !entry->evaluation_function )
            break;
        if ( ((1 << evaluation_context->evaluation_mode) & entry->evaluation_mode_mask) != 0 )
            result = entry->evaluation_function(actor_index, evaluation_context, firing_position);
        ++entry;
    }
    while ( (unsigned char)result );
    return result;
}
