/* scripted_sound_time_evaluate @0x8372C7B8 — HaloScript builtin evaluator. Argument [0] = sound definition
 * (long). Returns the remaining play time in ticks. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern int scripted_sound_time(int definition_index);
void scripted_sound_time_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { hs_return(thread_index, scripted_sound_time(arguments[0])); }
}
