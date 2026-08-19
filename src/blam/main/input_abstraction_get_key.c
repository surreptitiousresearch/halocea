/* input_abstraction_get_key @0x837256D0 — reads a key stroke only when the input abstraction
 * is in the text/UI mode that owns the keyboard. The active_modes bitfield selects a mode index
 * (8 -> 3, 4 -> 2, 2 -> 1, exactly 1 -> 0, else 255); only mode 2 forwards to input_get_key. */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"
#include "headers/input_abstraction_mode_flags.h"
#include "headers/key_stroke.h"

extern uint8_t input_get_key(key_stroke *key);

extern int input_abstraction_get_current_mode(void);
uint8_t input_abstraction_get_key(key_stroke *key)
{
    /* DEVIATION: verbatim inlined copy of input_abstraction_get_current_mode@0x83724BD8 (zero-xref donor) collapsed to a direct call; donor takes no params so no arg folding is needed, only the local's width (int -> __int16) differs, a compiler register-allocation artifact of inlining. */
    int mode = input_abstraction_get_current_mode();

    if (mode == _input_abstraction_mode_keyboard_bit)
        return input_get_key(key);
    return 0;
}
