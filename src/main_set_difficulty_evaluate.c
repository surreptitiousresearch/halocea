/* main_set_difficulty_evaluate @0x8372BCE8 — HaloScript wrapper for (main_set_difficulty <short>). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void main_set_difficulty(int16_t difficulty);
extern void hs_return(uint16_t thread_index, int value);

void main_set_difficulty_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    __int16 *difficulty = (__int16 *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( difficulty )
    {
        main_set_difficulty(*difficulty);
        hs_return(thread_index, 0);
    }
}
