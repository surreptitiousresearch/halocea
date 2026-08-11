/* scripted_sound_predict_evaluate @0x8372C718 — HaloScript builtin evaluator. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void scripted_sound_predict(int definition_index, uint8_t reference);
void scripted_sound_predict_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int * arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { scripted_sound_predict(arguments[0], ((unsigned char *)arguments)[4]); hs_return(thread_index, 0); }
}
