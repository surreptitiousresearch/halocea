/* main_set_map_name_evaluate @0x8372E640 — HaloScript wrapper for (main_set_map_name <string>): evaluate the argument expression, then apply it. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void main_set_map_name(const char *name);
extern void hs_return(uint16_t thread_index, int value);

void main_set_map_name_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    const char **value = (const char **)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( value )
    {
        main_set_map_name(*value);
        hs_return(thread_index, 0);
    }
}
