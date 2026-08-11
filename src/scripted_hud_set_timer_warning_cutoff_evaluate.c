/* scripted_hud_set_timer_warning_cutoff_evaluate @0x8372D7D8 — HaloScript builtin evaluator. Argument [0] = minutes (short), [4] = seconds
 * (short). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void scripted_hud_set_timer_warning_cutoff(int16_t minutes, int16_t seconds);
void scripted_hud_set_timer_warning_cutoff_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int16_t *arguments = (int16_t *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_hud_set_timer_warning_cutoff(arguments[0], arguments[2]); hs_return(thread_index, 0); }
}
