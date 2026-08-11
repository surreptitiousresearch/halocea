/* scripted_looping_sound_set_alternate_evaluate @0x8372C978 — HaloScript builtin evaluator. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void scripted_looping_sound_set_alternate(int definition_index, uint8_t alternate);
void scripted_looping_sound_set_alternate_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int * arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_looping_sound_set_alternate(arguments[0], ((unsigned char *)arguments)[4]); hs_return(thread_index, 0); }
}
