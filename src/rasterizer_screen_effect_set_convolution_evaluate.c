/* rasterizer_screen_effect_set_convolution_evaluate @0x8372DB58 — HaloScript builtin wrapper;
 * (short, short, real, real, real). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void rasterizer_screen_effect_set_convolution(int16_t convolution_extra_passes, int16_t convolution_type, float convolution_radius_lower_bound, float convolution_radius_upper_bound, float convolution_time);

void rasterizer_screen_effect_set_convolution_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* arguments is a raw HS argument block (packed wire) — faithful de-flagged reads */
        rasterizer_screen_effect_set_convolution(*(__int16 *)arguments, ((__int16 *)arguments)[2],
                                                 ((float *)arguments)[2], ((float *)arguments)[3],
                                                 ((float *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}
