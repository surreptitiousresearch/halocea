/* scripted_hud_show_crosshair_evaluate @0x8372D678 — HaloScript builtin evaluator. Argument [0] = boolean. No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void scripted_hud_show_crosshair(uint8_t show);
void scripted_hud_show_crosshair_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    unsigned char *arguments = (unsigned char *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_hud_show_crosshair(*arguments); hs_return(thread_index, 0); }
}
