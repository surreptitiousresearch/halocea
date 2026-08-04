/* sound_set_master_gain_evaluate @0x8372CAD0 — HaloScript builtin wrapper for sound_set_master_gain; takes a real. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void sound_set_master_gain(float gain);

void sound_set_master_gain_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        sound_set_master_gain(*(float *)arguments);
        hs_return(thread_index, 0);
    }
}
