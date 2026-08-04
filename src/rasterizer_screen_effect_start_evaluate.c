/* rasterizer_screen_effect_start_evaluate @0x8372DB10 — HaloScript builtin wrapper; takes a boolean. */

#include <stdint.h>

/* extern corrected to match def (hs_macro_function_evaluate.c): returns int*. */
extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void rasterizer_screen_effect_start(uint8_t clear);

void rasterizer_screen_effect_start_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        rasterizer_screen_effect_start((unsigned char)arguments[0]);
        hs_return(thread_index, 0);
    }
}
