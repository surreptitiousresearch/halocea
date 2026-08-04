/* virtual_keyboard_render @0x83783EE4 — render the on-screen virtual keyboard when it is active. */

#include "headers/virtual_keyboard_globals_t.h"
#include "headers/blam_data_globals.h"

extern void virtual_keyboard_render_internal(void);

void virtual_keyboard_render(void)
{
    if (virtual_keyboard_globals.active)
        virtual_keyboard_render_internal();
}
