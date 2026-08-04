/* scenery_animation_start_at_frame_evaluate @0x837283C8 — HaloScript builtin wrapper; (object, animation graph, string, short frame). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void scenery_animation_start_at_frame(int scenery_index, int animation_graph_index, const char *animation_name, int16_t frame_index);

void scenery_animation_start_at_frame_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        scenery_animation_start_at_frame(arguments[0], arguments[1], (const char *)arguments[2], ((__int16 *)arguments)[6]);
        hs_return(thread_index, 0);
    }
}
