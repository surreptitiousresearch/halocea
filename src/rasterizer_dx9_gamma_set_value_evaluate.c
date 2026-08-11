/* rasterizer_dx9_gamma_set_value_evaluate @0x8372D980 — HaloScript builtin evaluator. Argument block: [0] = value. No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void rasterizer_dx9_gamma_set_value(int value);

void rasterizer_dx9_gamma_set_value_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        rasterizer_dx9_gamma_set_value(arguments[0]);
        hs_return(thread_index, 0);
    }
}
