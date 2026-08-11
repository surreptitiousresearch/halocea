/* unit_scripting_start_user_animation_list_evaluate @0x837286A0 — HaloScript builtin wrapper; packs a boolean result. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern uint8_t unit_scripting_start_user_animation_list(int object_list_index, int animation_graph_index, const char *animation_name, uint8_t interpolate);

void unit_scripting_start_user_animation_list_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *(unsigned char *)&result = unit_scripting_start_user_animation_list(
            /* HS packed arg block: byte +12 is the boolean interpolate arg (BE high byte of arguments[3]) */
            arguments[0], arguments[1], (const char *)arguments[2], ((unsigned char *)arguments)[12]);
        hs_return(thread_index, result);
    }
}
