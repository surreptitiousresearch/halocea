/* global_pre_evaluator_table @ 0x821285F8 — .rdata, 56 bytes = 7 x pre_evaluator_table_entry
 * (sizeof == 8; 56 / 8 == 7, no padding). firing_position_pre_evaluate walks it, running every
 * entry whose evaluation_mode_mask has the bit of the context's firing_point_evaluation_mode set,
 * and stops at the entry whose evaluation_function is null.
 * Initializer reconstructed from the binary (big-endian word pairs, mask | pad, function):
 *   [0] 0xFFFF0000 0x837EFE90 -> mask -1 (every mode)          pre_evaluator_global
 *   [1] 0x00090000 0x837EEA50 -> mask 0x0009 fight|uncover      pre_evaluator_attack
 *   [2] 0x004D0000 0x837EED08 -> mask 0x004D fight|cover|uncover|avoid  pre_evaluator_combatmove
 *   [3] 0x00100000 0x837EE9A8 -> mask 0x0010 guard              pre_evaluator_guard
 *   [4] 0x00020000 0x837EE7A8 -> mask 0x0002 panic              pre_evaluator_panic
 *   [5] 0x00200000 0x837EE650 -> mask 0x0020 pursue             pre_evaluator_pursuit
 *   [6] 0x00000000 0x00000000 -> terminator
 *
 * The object is in .rdata; the corpus declaration in src/firing_position_pre_evaluate.c is
 * non-const and the definition matches it so every declaration of the symbol agrees.
 */

#include <stdint.h>
#include "../headers/pre_evaluator_table_entry.h"
#include "../headers/firing_point_evaluation_mode.h"

/* Every callee is declared with the funcptr-SLOT signature taken from
 * pre_evaluator_table_entry.evaluation_function rather than the concrete spelling of its
 * definition TU; a slot/definition mismatch here would raise C4113. */
extern void pre_evaluator_global(int actor_index, firing_position_evaluation_context *evaluation_context, int16_t firing_position_count, firing_position *firing_positions);      /* 0x837EFE90 */
extern void pre_evaluator_attack(int actor_index, firing_position_evaluation_context *evaluation_context, int16_t firing_position_count, firing_position *firing_positions);      /* 0x837EEA50 */
extern void pre_evaluator_combatmove(int actor_index, firing_position_evaluation_context *evaluation_context, int16_t firing_position_count, firing_position *firing_positions);  /* 0x837EED08 */
extern void pre_evaluator_guard(int actor_index, firing_position_evaluation_context *evaluation_context, int16_t firing_position_count, firing_position *firing_positions);       /* 0x837EE9A8 */
extern void pre_evaluator_panic(int actor_index, firing_position_evaluation_context *evaluation_context, int16_t firing_position_count, firing_position *firing_positions);       /* 0x837EE7A8 */
extern void pre_evaluator_pursuit(int actor_index, firing_position_evaluation_context *evaluation_context, int16_t firing_position_count, firing_position *firing_positions);     /* 0x837EE650 */

pre_evaluator_table_entry global_pre_evaluator_table[7] =
{
    { -1, { 0 }, pre_evaluator_global },   /* every evaluation mode */
    { (1 << _firing_point_evaluation_mode_fight)
    | (1 << _firing_point_evaluation_mode_uncover), { 0 }, pre_evaluator_attack },
    { (1 << _firing_point_evaluation_mode_fight)
    | (1 << _firing_point_evaluation_mode_cover)
    | (1 << _firing_point_evaluation_mode_uncover)
    | (1 << _firing_point_evaluation_mode_avoid), { 0 }, pre_evaluator_combatmove },
    { (1 << _firing_point_evaluation_mode_guard),  { 0 }, pre_evaluator_guard },
    { (1 << _firing_point_evaluation_mode_panic),  { 0 }, pre_evaluator_panic },
    { (1 << _firing_point_evaluation_mode_pursue), { 0 }, pre_evaluator_pursuit },
    { 0, { 0 }, nullptr },                 /* terminator */
};
