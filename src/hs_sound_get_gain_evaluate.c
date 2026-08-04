/* hs_sound_get_gain_evaluate @0x837279D0 — HaloScript builtin evaluator. Argument [0] = sound tag name
 * (string). Returns the gain as a float (re-interpreted into the result word). */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern float hs_sound_get_gain(const char *tag_name);
void hs_sound_get_gain_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    char **arguments = (char **)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        int result;
        *(float *)&result = hs_sound_get_gain(*arguments);
        hs_return(thread_index, result);
    }
}
