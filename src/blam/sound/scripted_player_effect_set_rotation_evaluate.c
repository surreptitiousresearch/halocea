/* scripted_player_effect_set_rotation_evaluate @0x8372D390 — HaloScript builtin evaluator. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void scripted_player_effect_set_rotation(float yaw, float pitch, float roll);
void scripted_player_effect_set_rotation_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    float * arguments = (float *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_player_effect_set_rotation(arguments[0], arguments[1], arguments[2]); hs_return(thread_index, 0); }
}
