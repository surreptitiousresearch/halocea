#pragma once
/* pre_evaluator_table_entry — one entry in the firing-position pre-evaluation dispatch table. The mask
 * selects which evaluation modes the function runs for. DB layout. */

#include "firing_position_evaluation_context.h"
#include "firing_position.h"

typedef struct pre_evaluator_table_entry
{
    __int16 evaluation_mode_mask;   /* 0x0 */
    unsigned char _pad02[2];        /* 0x2 — db-verified padding */
    void (__fastcall *evaluation_function)(int actor_index, firing_position_evaluation_context *context, __int16 firing_position_count, firing_position *firing_positions); /* 0x4 */
} pre_evaluator_table_entry;        /* 0x8 */
