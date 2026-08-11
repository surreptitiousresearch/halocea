/* scripted_hud_set_objective_evaluate @0x8372D740 — HaloScript builtin evaluator. Argument [0] = message index (short). No return
 * value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void scripted_hud_set_objective(int16_t message_index);
void scripted_hud_set_objective_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int16_t *arguments = (int16_t *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_hud_set_objective(*arguments); hs_return(thread_index, 0); }
}
