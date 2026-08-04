/* rasterizer_screen_effect_set_filter_desaturation_tint_evaluate @0x8372DC18 — HaloScript builtin wrapper;
 * (real, real, real). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void rasterizer_screen_effect_set_filter_desaturation_tint(float red, float green, float blue);

void rasterizer_screen_effect_set_filter_desaturation_tint_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS argument block: packed 4-byte real slots */
        rasterizer_screen_effect_set_filter_desaturation_tint(((float *)arguments)[0], ((float *)arguments)[1],
                                                              ((float *)arguments)[2]);
        hs_return(thread_index, 0);
    }
}
