/* player_input_enable_evaluate @0x8372B778 — HaloScript builtin wrapper for player_input_enable. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void player_input_enable(uint8_t enable);

void player_input_enable_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    unsigned char *arguments = (unsigned char *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        player_input_enable(*arguments);
        hs_return(thread_index, 0);
    }
}
