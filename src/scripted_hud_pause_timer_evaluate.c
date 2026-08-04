/* scripted_hud_pause_timer_evaluate @0x8372D8C0 — HaloScript builtin evaluator. Argument [0] = boolean
 * paused. No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void scripted_hud_pause_timer(uint8_t paused);
void scripted_hud_pause_timer_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    unsigned char *arguments = (unsigned char *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_hud_pause_timer(*arguments); hs_return(thread_index, 0); }
}
