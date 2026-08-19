/* unit_scripting_has_weapon_readied_evaluate @0x83728F68 — HaloScript builtin wrapper for unit_scripting_has_weapon_readied; packs a boolean result. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern uint8_t unit_scripting_has_weapon_readied(int unit_index, int weapon_definition_index);

void unit_scripting_has_weapon_readied_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* DEVIATION: endian-portable respelling of the BE high-byte store (was an *(narrow*)&result pun; hs_inspect_boolean extracts value >> 24) 2026-08-18 */
        int result = (int)((uint8_t)(unit_scripting_has_weapon_readied(arguments[0], arguments[1]))) << 24;
        hs_return(thread_index, result);
    }
}
