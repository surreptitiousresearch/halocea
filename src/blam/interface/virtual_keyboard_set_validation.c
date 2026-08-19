/* virtual_keyboard_set_validation @0x837833F0 */
#include "headers/virtual_keyboard_globals_t.h"

void virtual_keyboard_set_validation(int validate_mode)
{
    virtual_keyboard_globals.validate_mode = validate_mode;
}
