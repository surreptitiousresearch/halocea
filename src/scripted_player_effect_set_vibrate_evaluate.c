/* scripted_player_effect_set_vibrate_evaluate @0x8372D3E0 — HaloScript builtin evaluator. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void scripted_player_effect_set_vibrate(float left_frequency, float right_frequency);
void scripted_player_effect_set_vibrate_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    float * arguments = (float *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_player_effect_set_vibrate(arguments[0], arguments[1]); hs_return(thread_index, 0); }
}
