/* rasterizer_set_near_clip_distance_evaluate @0x8372DCF0 — HaloScript builtin wrapper; takes a real. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void rasterizer_set_near_clip_distance(float near_clip_distance);

void rasterizer_set_near_clip_distance_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        rasterizer_set_near_clip_distance(*(float *)arguments);
        hs_return(thread_index, 0);
    }
}
