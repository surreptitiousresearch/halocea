/* player0_look_invert_pitch_evaluate @0x8372DDD0 — HaloScript builtin wrapper for player0_look_invert_pitch. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void player0_look_invert_pitch(uint8_t invert);

void player0_look_invert_pitch_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        player0_look_invert_pitch(((unsigned char *)arguments)[0]);
        hs_return(thread_index, 0);
    }
}
