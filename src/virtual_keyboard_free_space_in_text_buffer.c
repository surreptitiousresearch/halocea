/* virtual_keyboard_free_space_in_text_buffer @0x83783E08 */
#include "headers/virtual_keyboard_globals_t.h"

extern unsigned int ustrlen(const wchar_t *string);

unsigned int virtual_keyboard_free_space_in_text_buffer(void)
{
    return virtual_keyboard_globals.buffer_size - 2 * (ustrlen(virtual_keyboard_globals.text_buffer) + 1);
}
