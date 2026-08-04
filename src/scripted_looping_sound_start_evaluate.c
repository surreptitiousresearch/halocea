/* scripted_looping_sound_start_evaluate @0x8372C890 — HaloScript builtin evaluator. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void scripted_looping_sound_start(int definition_index, int source_object_index, float scale);
void scripted_looping_sound_start_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int * arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_looping_sound_start(arguments[0], arguments[1], ((float *)arguments)[2]); hs_return(thread_index, 0); }
}
