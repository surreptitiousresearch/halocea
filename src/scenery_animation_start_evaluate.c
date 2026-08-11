/* scenery_animation_start_evaluate @0x83728378 — HaloScript builtin wrapper; (object, animation graph, string). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void scenery_animation_start(int scenery_index, int animation_graph_index, const char *animation_name);

void scenery_animation_start_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        scenery_animation_start(arguments[0], arguments[1], (const char *)arguments[2]);
        hs_return(thread_index, 0);
    }
}
