/* hs_sound_set_gain_evaluate @0x83727980 — HaloScript builtin evaluator. Argument [0] = sound tag name
 * (string), [4] = gain (float). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void hs_sound_set_gain(const char *tag_name, float gain);
void hs_sound_set_gain_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { hs_sound_set_gain((const char *)arguments[0], ((float *)arguments)[1]); hs_return(thread_index, 0); }
}
