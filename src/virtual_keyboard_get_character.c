/* virtual_keyboard_get_character @0x837838A0 — resolve a virtual-keyboard keycode to the character for the
 * current shift/caps/symbols modifier combination (each 80-byte virtual_key record holds several wchar_t
 * alternates at word offsets 1-7, selected below). Returns 127 (DEL) if the resolved slot is the
 * null/unmapped placeholder.
 *
 * DEVIATION: the decompiler rendered the per-key record stride as two separate additive terms
 * (`16*(u16)keycode + 16*((4*keycode)&0x3FFFC)`); disasm shows these combine to a single 80-byte stride
 * (`(u16)keycode + 4*(u16)keycode) * 16 = 80*keycode`), not the 16-byte stride a naive reading suggests.
 * That stride is sizeof(virtual_key), so the record is indexed as a typed array below. */

#include <stdint.h>
#include "headers/virtual_keyboard_globals_t.h"
#include "headers/virtual_key.h"

wchar_t virtual_keyboard_get_character(uint16_t keycode)
{
    const virtual_key *key =
        (const virtual_key *)virtual_keyboard_globals.keyboard->virtual_keys.address + keycode;

    wchar_t character;
    if ( virtual_keyboard_globals.shift_active )
    {
        if ( virtual_keyboard_globals.caps_active )
            character = key->shift_caps_character;
        else if ( virtual_keyboard_globals.symbols_active )
            character = key->shift_symbols_character;
        else
            character = key->shift_character;
    }
    else if ( virtual_keyboard_globals.caps_active )
    {
        character = virtual_keyboard_globals.symbols_active ? key->caps_symbols_character : key->caps_character;
    }
    else if ( virtual_keyboard_globals.symbols_active )
    {
        character = key->symbols_character;
    }
    else
    {
        character = key->lowercase_character;
    }

    return character ? character : 127;
}
