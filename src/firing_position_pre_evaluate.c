/* firing_position_pre_evaluate @0x837EF6A8 — run every registered pre-evaluation pass over a batch of
 * candidate firing positions. Each table entry whose mode mask includes the context's evaluation mode is
 * invoked to compute per-position pre-scores. No-op if the dispatch table is empty.
 *
 * DEVIATION: the guard was reconstructed as `extern unsigned char pre_evaluator_global;` - but
 * pre_evaluator_global is a FUNCTION (0x837EFE90), and it is global_pre_evaluator_table[0]'s
 * evaluation_function (reloc dump of 0x821285F8+4). The binary loads that table slot and tests it for
 * null (`lwz r10, pre_evaluator_table_entry.evaluation_function(r11)` / `cmplwi` / `beq`,
 * 0x837EF6CC-0x837EF6D4), i.e. an empty-table check, not a separate enable byte. */

#include "headers/pre_evaluator_table_entry.h"

extern pre_evaluator_table_entry global_pre_evaluator_table[];

void firing_position_pre_evaluate(int actor_index, firing_position_evaluation_context *evaluation_context,
                                  int firing_position_count, firing_position *firing_positions)
{
    if ( !global_pre_evaluator_table[0].evaluation_function )
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
