/* scripted_hud_set_timer_position_evaluate @0x8372D828 — HaloScript builtin evaluator. Argument [0] = x
 * (short), [4] = y (short), [8] = corner (short). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void scripted_hud_set_timer_position(int16_t x, int16_t y, int16_t corner);
void scripted_hud_set_timer_position_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int16_t *arguments = (int16_t *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_hud_set_timer_position(arguments[0], arguments[2], arguments[4]); hs_return(thread_index, 0); }
}
