/* scripted_camera_set_animation_evaluate @0x8372B498 — HaloScript builtin evaluator. Argument [0] = animation
 * graph tag (long), [4] = animation name (string). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void scripted_camera_set_animation(unsigned int animation_graph_index, const char *animation_name);
void scripted_camera_set_animation_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_camera_set_animation(arguments[0], (const char *)arguments[1]); hs_return(thread_index, 0); }
}
