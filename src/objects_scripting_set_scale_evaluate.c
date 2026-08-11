/* objects_scripting_set_scale_evaluate @0x83727F18 — HaloScript builtin evaluator. Argument [0] = object
 * index (long), [4] = scale (float), [8] = interpolation frame count (short). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void objects_scripting_set_scale(int object_index, float scale, int16_t frame_count);
void objects_scripting_set_scale_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        objects_scripting_set_scale(arguments[0], ((float *)arguments)[1], ((int16_t *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}
