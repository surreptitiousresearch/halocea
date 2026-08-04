/* game_time_set_speed_evaluate @0x8372E5F8 — HaloScript builtin evaluator. Argument block: [0] = speed
 * multiplier (float). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void game_time_set_speed(float speed);

void game_time_set_speed_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    float *arguments = (float *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        game_time_set_speed(*arguments);
        hs_return(thread_index, 0);
    }
}
