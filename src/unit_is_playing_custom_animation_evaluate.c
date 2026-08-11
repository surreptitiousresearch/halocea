/* unit_is_playing_custom_animation_evaluate @0x83728768 — HaloScript builtin wrapper for unit_is_playing_custom_animation; packs a boolean result. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern uint8_t unit_is_playing_custom_animation(int unit_index);

void unit_is_playing_custom_animation_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *(unsigned char *)&result = unit_is_playing_custom_animation(arguments[0]);
        hs_return(thread_index, result);
    }
}
