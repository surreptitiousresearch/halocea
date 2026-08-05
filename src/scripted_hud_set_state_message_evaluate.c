/* scripted_hud_set_state_message_evaluate @0x8372D6F8 — HaloScript builtin evaluator. Argument [0] = message index (short). No return
 * value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void scripted_hud_set_state_message(int16_t message_index);
void scripted_hud_set_state_message_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int16_t *arguments = (int16_t *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_hud_set_state_message(*arguments); hs_return(thread_index, 0); }
}
