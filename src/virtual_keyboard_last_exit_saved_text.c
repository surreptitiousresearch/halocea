/* virtual_keyboard_last_exit_saved_text @0x83783418 */
#include <stdint.h>
#include "headers/virtual_keyboard_globals_t.h"

uint8_t virtual_keyboard_last_exit_saved_text(void)
{
    return virtual_keyboard_globals.buffer_saved;
}
