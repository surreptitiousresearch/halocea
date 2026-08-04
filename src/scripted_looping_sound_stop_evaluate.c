/* scripted_looping_sound_stop_evaluate @0x8372C8E0 — HaloScript builtin evaluator. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void scripted_looping_sound_stop(int definition_index);
void scripted_looping_sound_stop_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int * arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_looping_sound_stop(arguments[0]); hs_return(thread_index, 0); }
}
