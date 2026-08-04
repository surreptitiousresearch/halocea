/* rasterizer_screen_effect_set_filter_evaluate @0x8372DBB8 — HaloScript builtin wrapper for
 * rasterizer_screen_effect_set_filter.
 *
 * DEVIATION: the decompiler rendered the additive boolean as (unsigned __int8)pointer; the disassembly loads
 * it from a byte at argument offset 0x10, reconstructed as *((unsigned char *)arguments + 16). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void rasterizer_screen_effect_set_filter(float filter_light_enhancement_intensity_lower_bound, float filter_light_enhancement_intensity_upper_bound, float filter_desaturation_intensity_lower_bound, float filter_desaturation_intensity_upper_bound, uint8_t filter_desaturation_is_additive, float filter_time);

void rasterizer_screen_effect_set_filter_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* arguments is a raw HS argument block (packed wire) — faithful de-flagged reads */
        rasterizer_screen_effect_set_filter(*(float *)arguments, ((float *)arguments)[1],
                                            ((float *)arguments)[2], ((float *)arguments)[3],
                                            ((unsigned char *)arguments)[16], ((float *)arguments)[5]);
        hs_return(thread_index, 0);
    }
}
