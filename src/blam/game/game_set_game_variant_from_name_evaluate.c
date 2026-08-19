/* game_set_game_variant_from_name_evaluate @0x8372B630 — HaloScript builtin evaluator. Argument block:
 * [0] = variant name (string pointer). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void game_set_game_variant_from_name(const char *name);

void game_set_game_variant_from_name_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    char **arguments = (char **)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        game_set_game_variant_from_name(*arguments);
        hs_return(thread_index, 0);
    }
}
