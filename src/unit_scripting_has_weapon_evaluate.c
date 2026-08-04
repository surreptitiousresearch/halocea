/* unit_scripting_has_weapon_evaluate @0x83728F10 — HaloScript builtin wrapper for unit_scripting_has_weapon; packs a boolean result. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern uint8_t unit_scripting_has_weapon(int unit_index, int weapon_definition_index);

void unit_scripting_has_weapon_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *((unsigned char *)&result + 3) = unit_scripting_has_weapon(arguments[0], arguments[1]);
        hs_return(thread_index, result);
    }
}
