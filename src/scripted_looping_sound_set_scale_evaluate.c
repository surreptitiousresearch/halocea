/* scripted_looping_sound_set_scale_evaluate @0x8372C928 — HaloScript builtin evaluator. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void scripted_looping_sound_set_scale(int definition_index, float scale);
void scripted_looping_sound_set_scale_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int * arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_looping_sound_set_scale(arguments[0], ((float *)arguments)[1]); hs_return(thread_index, 0); }
}
