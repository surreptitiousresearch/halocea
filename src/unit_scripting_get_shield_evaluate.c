/* unit_scripting_get_shield_evaluate @0x83728E68 — HaloScript builtin wrapper for unit_scripting_get_shield; packs a real result. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern float unit_scripting_get_shield(int unit_index);

void unit_scripting_get_shield_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *(float *)&result = unit_scripting_get_shield(arguments[0]);
        hs_return(thread_index, result);
    }
}
