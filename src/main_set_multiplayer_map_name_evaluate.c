/* main_set_multiplayer_map_name_evaluate @0x8372BCA0 — HaloScript wrapper for (main_set_multiplayer_map_name <string>):
 * evaluate the macro's argument expression, then apply it. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void main_set_multiplayer_map_name(const char *name);
extern void hs_return(uint16_t thread_index, int value);

void main_set_multiplayer_map_name_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    const char **name = (const char **)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( name )
    {
        main_set_multiplayer_map_name(*name);
        hs_return(thread_index, 0);
    }
}
