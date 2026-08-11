/* rasterizer_script_screen_effect_set_value_evaluate @0x8372DAC0 — HaloScript builtin wrapper; (short, real). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void rasterizer_script_screen_effect_set_value(int16_t index, float value);

void rasterizer_script_screen_effect_set_value_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS argument block: packed 4-byte slots (short arg in slot 0, real in slot 1) */
        rasterizer_script_screen_effect_set_value(*(int16_t *)arguments, ((float *)arguments)[1]);
        hs_return(thread_index, 0);
    }
}
