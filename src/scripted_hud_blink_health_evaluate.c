/* scripted_hud_blink_health_evaluate @0x8372D510 — HaloScript builtin evaluator. Argument [0] = boolean. No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void scripted_hud_blink_health(uint8_t blink);
void scripted_hud_blink_health_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    unsigned char *arguments = (unsigned char *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_hud_blink_health(*arguments); hs_return(thread_index, 0); }
}
