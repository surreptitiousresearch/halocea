/* scripted_player_effect_start_evaluate @0x8372D430 — HaloScript builtin evaluator. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void scripted_player_effect_start(float max_intensity, float attack);
void scripted_player_effect_start_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    float * arguments = (float *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_player_effect_start(arguments[0], arguments[1]); hs_return(thread_index, 0); }
}
