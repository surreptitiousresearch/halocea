/* scripted_camera_set_dead_evaluate @0x8372B530 — HaloScript builtin evaluator. Argument [0] = unit index (long). No return
 * value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void scripted_camera_set_dead(int unit_index);
void scripted_camera_set_dead_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_camera_set_dead(arguments[0]); hs_return(thread_index, 0); }
}
