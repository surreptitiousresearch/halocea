/* scripted_player_effect_stop_evaluate @0x8372D480 — HaloScript builtin evaluator. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void scripted_player_effect_stop(float decay);
void scripted_player_effect_stop_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    float * arguments = (float *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_player_effect_stop(*arguments); hs_return(thread_index, 0); }
}
