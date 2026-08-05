/* unit_scripting_get_grenade_count_evaluate @0x83728EB8 — HaloScript builtin wrapper for
 * unit_scripting_get_grenade_count; packs a short result. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern int unit_scripting_get_grenade_count(int unit_index);

void unit_scripting_get_grenade_count_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result;
    *((int16_t *)&result) = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *((int16_t *)&result + 1) = unit_scripting_get_grenade_count(arguments[0]);
        hs_return(thread_index, result);
    }
}
