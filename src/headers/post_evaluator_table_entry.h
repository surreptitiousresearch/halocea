#pragma once
/* post_evaluator_table_entry — one entry in the firing-position post-evaluation dispatch table. The
 * function returns whether the position remains acceptable. DB layout. */

#include "firing_position_evaluation_context.h"
#include "firing_position.h"

typedef struct post_evaluator_table_entry
{
    __int16 evaluation_mode_mask;   /* 0x0 */
    unsigned char _pad02[2];        /* 0x2 — db-verified padding */
    unsigned __int8 (__fastcall *evaluation_function)(int actor_index, firing_position_evaluation_context *context, firing_position *firing_position); /* 0x4 */
} post_evaluator_table_entry;       /* 0x8 */
