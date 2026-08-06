/* global_post_evaluator_table @ 0x82128630 (.rdata, 48 bytes)
 * DB applied_types: const post_evaluator_table_entry global_post_evaluator_table[6];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   [ 0] +0x00 evaluation_mode_mask       = 0xFFFF
 *        +0x04 evaluation_function        = 0x837EF110 -> post_evaluator_global
 *   [ 1] +0x00 evaluation_mode_mask       = 0x0051
 *        +0x04 evaluation_function        = 0x837EF5A0 -> post_evaluator_attack
 *   [ 2] +0x00 evaluation_mode_mask       = 0x0008
 *        +0x04 evaluation_function        = 0x837EF4D0 -> post_evaluator_uncover
 *   [ 3] +0x00 evaluation_mode_mask       = 0x0006
 *        +0x04 evaluation_function        = 0x837EF3F8 -> post_evaluator_hide
 *   [ 4] +0x00 evaluation_mode_mask       = 0x0020
 *        +0x04 evaluation_function        = 0x837EF220 -> post_evaluator_pursuit
 *   [ 5] +0x00 evaluation_mode_mask       = 0x0000
 *        +0x04 evaluation_function        = 0x00000000
 * .rdata, 48 bytes = 6 x post_evaluator_table_entry
 * (sizeof == 8; 48 / 8 == 6, no padding). firing_position_post_evaluate (and
 * firing_positions_get_post_evaluation_bound / actor_select_firing_position) walk it, running
 * every entry whose evaluation_mode_mask has the bit of the context's firing_point_evaluation_mode
 * set, and stop at the entry whose evaluation_function is null.
 * Initializer reconstructed from the binary (big-endian word pairs, mask | pad, function):
 *   [0] 0xFFFF0000 0x837EF110 -> mask -1 (every mode)             post_evaluator_global
 *   [1] 0x00510000 0x837EF5A0 -> mask 0x0051 fight|guard|avoid    post_evaluator_attack
 *   [2] 0x00080000 0x837EF4D0 -> mask 0x0008 uncover              post_evaluator_uncover
 *   [3] 0x00060000 0x837EF3F8 -> mask 0x0006 panic|cover          post_evaluator_hide
 *   [4] 0x00200000 0x837EF220 -> mask 0x0020 pursue               post_evaluator_pursuit
 *   [5] 0x00000000 0x00000000 -> terminator
 * The object is in .rdata; all three corpus declarations are non-const and the definition matches
 * them so every declaration of the symbol agrees.
 * /
 */
#include <stdint.h>
#include "../headers/post_evaluator_table_entry.h"
#include "../headers/firing_point_evaluation_mode.h"

/* Every callee is declared with the funcptr-SLOT signature taken from
 * post_evaluator_table_entry.evaluation_function rather than the concrete spelling of its
 * definition TU (post_evaluator_pursuit returns int, not uint8_t); the concrete spelling
 * mismatches the slot and would raise C4113. */
extern uint8_t post_evaluator_global(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position);  /* 0x837EF110 */
extern uint8_t post_evaluator_attack(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position);  /* 0x837EF5A0 */
extern uint8_t post_evaluator_uncover(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position); /* 0x837EF4D0 */
extern uint8_t post_evaluator_hide(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position);    /* 0x837EF3F8 */
extern uint8_t post_evaluator_pursuit(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position); /* 0x837EF220 */

/* Field order per row (post_evaluator_table_entry, DB-verified layout):
   evaluation_mode_mask, _pad02, evaluation_function. */
post_evaluator_table_entry global_post_evaluator_table[6] =
{
    { -1, { 0 }, post_evaluator_global },   /* every evaluation mode */
    { (1 << _firing_point_evaluation_mode_fight)
    | (1 << _firing_point_evaluation_mode_guard)
    | (1 << _firing_point_evaluation_mode_avoid), { 0 }, post_evaluator_attack },
    { (1 << _firing_point_evaluation_mode_uncover), { 0 }, post_evaluator_uncover },
    { (1 << _firing_point_evaluation_mode_panic)
    | (1 << _firing_point_evaluation_mode_cover),   { 0 }, post_evaluator_hide },
    { (1 << _firing_point_evaluation_mode_pursue),  { 0 }, post_evaluator_pursuit },
    { 0, { 0 }, nullptr },                  /* terminator */
};
