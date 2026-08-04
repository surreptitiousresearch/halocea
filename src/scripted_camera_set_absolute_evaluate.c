/* scripted_camera_set_absolute_evaluate @0x8372B3F8 — HaloScript builtin evaluator. Argument [0] = camera
 * point index (short), [4] = duration ticks (short). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void scripted_camera_set_absolute(int16_t camera_point_index, int16_t tick_count);
void scripted_camera_set_absolute_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    __int16 *arguments = (__int16 *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_camera_set_absolute(arguments[0], arguments[2]); hs_return(thread_index, 0); }
}
