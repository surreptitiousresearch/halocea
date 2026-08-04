/* firing_position_pre_evaluate @0x837EF6A8 — run every registered pre-evaluation pass over a batch of
 * candidate firing positions. Each table entry whose mode mask includes the context's evaluation mode is
 * invoked to compute per-position pre-scores. No-op if pre-evaluation is globally disabled. */

#include "headers/pre_evaluator_table_entry.h"

extern unsigned char pre_evaluator_global;
extern pre_evaluator_table_entry global_pre_evaluator_table[];

void firing_position_pre_evaluate(int actor_index, firing_position_evaluation_context *evaluation_context,
                                  int firing_position_count, firing_position *firing_positions)
{
    if ( !pre_evaluator_global )
        return;

    pre_evaluator_table_entry *entry = global_pre_evaluator_table;
    do
    {
        if ( ((1 << evaluation_context->evaluation_mode) & entry->evaluation_mode_mask) != 0 )
            entry->evaluation_function(actor_index, evaluation_context, firing_position_count, firing_positions);
        ++entry;
    }
    while ( entry->evaluation_function );
}
