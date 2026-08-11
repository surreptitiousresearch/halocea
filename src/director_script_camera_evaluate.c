/* director_script_camera_evaluate @0x8372B3B0 — HaloScript builtin evaluator. Argument block:
 * [0] = boolean enable. No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void director_script_camera(uint8_t enabled);

void director_script_camera_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    unsigned char *arguments = (unsigned char *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        director_script_camera(*arguments);
        hs_return(thread_index, 0);
    }
}
