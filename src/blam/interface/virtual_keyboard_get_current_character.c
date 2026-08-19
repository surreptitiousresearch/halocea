/* virtual_keyboard_get_current_character @0x83783950 — resolve the currently-highlighted key's character
 * variant for the active shift/caps/symbols combination, falling back to DEL (127) if that variant is the
 * null key. */

#include <wchar.h>
#include <stdint.h>
#include "headers/virtual_keyboard_globals_t.h"
#include "headers/virtual_key.h"
#include "headers/blam_data_globals.h"

extern const char virtual_keyboard_layout_table[5][11];

extern wchar_t virtual_keyboard_get_character(uint16_t keycode);
wchar_t virtual_keyboard_get_current_character(void)
{
    uint16_t key_code = virtual_keyboard_layout_table[virtual_keyboard_globals.row][virtual_keyboard_globals.column];
    /* DEVIATION: body was an inlined copy of virtual_keyboard_get_character@0x837838A0 (field-for-field identical shift/caps/symbols walk + DEL fallback); collapsed to a call, key_code passed as the keycode arg. */
    return virtual_keyboard_get_character(key_code);
}
