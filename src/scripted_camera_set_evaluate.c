/* scripted_camera_set_evaluate @0x8372B448 — HaloScript builtin evaluator. Argument [0] = camera point index
 * (short), [4] = duration ticks (short), [8] = object to anchor to (long). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void scripted_camera_set(int16_t camera_point_index, int16_t tick_count, int relative_to_object_index);
void scripted_camera_set_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        scripted_camera_set(*(int16_t *)arguments, ((int16_t *)arguments)[2], arguments[2]);
        hs_return(thread_index, 0);
    }
}
