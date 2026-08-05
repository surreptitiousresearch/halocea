/* scripted_hud_set_timer_time_evaluate @0x8372D788 — HaloScript builtin evaluator. Argument [0] = minutes (short), [4] = seconds
 * (short). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void scripted_hud_set_timer_time(int16_t minutes, int16_t seconds);
void scripted_hud_set_timer_time_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int16_t *arguments = (int16_t *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_hud_set_timer_time(arguments[0], arguments[2]); hs_return(thread_index, 0); }
}
