/* unit_scripting_has_weapon_evaluate @0x83728F10 — HaloScript builtin wrapper for unit_scripting_has_weapon;
 * packs a boolean result into byte 0 of the 4-byte HaloScript value slot. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern uint8_t unit_scripting_has_weapon(int unit_index, int weapon_definition_index);

void unit_scripting_has_weapon_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* DEVIATION: `stb r3, 0x50(r1)` @0x83728F44 shares the displacement of the word-zeroing
         * `stw r11, 0x50(r1)` @0x83728F28 — byte offset +0, not +3. */
        /* DEVIATION: endian-portable respelling of the BE high-byte store (was an *(narrow*)&result pun; hs_inspect_boolean extracts value >> 24) 2026-08-18 */
        int result = (int)((uint8_t)(unit_scripting_has_weapon(arguments[0], arguments[1]))) << 24;
        hs_return(thread_index, result);
    }
}
