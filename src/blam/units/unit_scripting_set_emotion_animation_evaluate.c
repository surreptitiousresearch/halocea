/* unit_scripting_set_emotion_animation_evaluate @0x837289C0 — HaloScript builtin wrapper. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void unit_scripting_set_emotion_animation(int unit_index, const char *animation_name);

void unit_scripting_set_emotion_animation_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        unit_scripting_set_emotion_animation(arguments[0], (const char *)arguments[1]);
        hs_return(thread_index, 0);
    }
}
