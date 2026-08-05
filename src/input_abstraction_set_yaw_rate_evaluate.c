/* input_abstraction_set_yaw_rate_evaluate @0x8372E0B0 — HaloScript builtin wrapper for input_abstraction_set_yaw_rate; (short, real). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void input_abstraction_set_yaw_rate(int16_t local_player_index, float yaw_rate);

void input_abstraction_set_yaw_rate_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS argument block: packed wire, faithfully de-flagged */
        input_abstraction_set_yaw_rate(*(int16_t *)arguments, ((float *)arguments)[1]);
        hs_return(thread_index, 0);
    }
}
