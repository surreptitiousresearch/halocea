/* virtual_keyboard_get_character @0x837838CC — resolve a virtual-keyboard keycode to the character for the
 * current shift/caps/symbols modifier combination (each 80-byte virtual_key record holds several wchar_t
 * alternates at word offsets 1-7, selected below). Returns 127 (DEL) if the resolved slot is the
 * null/unmapped placeholder.
 *
 * DEVIATION: the decompiler rendered the per-key record stride as two separate additive terms
 * (`16*(u16)keycode + 16*((4*keycode)&0x3FFFC)`); disasm shows these combine to a single 80-byte stride
 * (`(u16)keycode + 4*(u16)keycode) * 16 = 80*keycode`), not the 16-byte stride a naive reading suggests. */

#include "headers/virtual_keyboard_globals_t.h"

wchar_t virtual_keyboard_get_character(unsigned __int16 keycode)
{
    unsigned __int16 *key = (unsigned __int16 *)((char *)virtual_keyboard_globals.keyboard->virtual_keys.address
        + 80 * keycode);

    unsigned __int16 character;
    if ( virtual_keyboard_globals.shift_active )
    {
        if ( virtual_keyboard_globals.caps_active )
            character = key[5];
        else if ( virtual_keyboard_globals.symbols_active )
            character = key[6];
        else
            character = key[2];
    }
    else if ( virtual_keyboard_globals.caps_active )
    {
        character = virtual_keyboard_globals.symbols_active ? key[7] : key[3];
    }
    else if ( virtual_keyboard_globals.symbols_active )
    {
        character = key[4];
    }
    else
    {
        character = key[1];
    }

    return character ? character : 127;
}
