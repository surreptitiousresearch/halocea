/* player_control_action_test_reset_evaluate @0x8372B818 — HaloScript builtin wrapper for player_control_action_test_reset.
 *
 * DEVIATION: player_control_action_test_reset is a void no-argument builtin; the decompiler renders it as taking the evaluator's
 * (function_index, thread_index, initialize) triple, so it is called that way here. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern void player_control_action_test_reset(void);

void player_control_action_test_reset_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    player_control_action_test_reset();
    hs_return(thread_index, 0);
}
