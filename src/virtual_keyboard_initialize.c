/* virtual_keyboard_initialize @0x83783290 — one-time setup of the on-screen virtual keyboard: clears the
 * active/shift/caps/symbols flags, resolves the "ui\\english" keyboard-layout tag ('kybd'), and if found,
 * resets the cursor position, clears the text-buffer/last-event state (buffer_size=0, last_event=-1,
 * written together as one DWORD store), caches the layout pointer, and clears the last-key/repeat/buffer
 * pointers. Finally resolves the caret bitmap tag ("ui\\shell\\bitmaps\\white"). Returns whether a keyboard
 * layout was found. */

#include <stdint.h>
#include "headers/virtual_keyboard_globals_t.h"
#include "headers/global_tag_instances.h"

extern int tag_loaded(uint32_t group_tag, const char *name);

int virtual_keyboard_initialize(void)
{
    virtual_keyboard_globals.active = 0;
    virtual_keyboard_globals.shift_active = 0;
    virtual_keyboard_globals.caps_active = 0;
    virtual_keyboard_globals.symbols_active = 0;

    int keyboard_tag = tag_loaded(0x76636B79u, "ui\\english");   /* 'vcky' (virtual_keyboard tag group; DB VIRTUAL_KEYBOARD_TAG) */
    if ( keyboard_tag != -1 )
    {
        virtual_keyboard_globals.row = 0;
        virtual_keyboard_globals.column = 0;
        virtual_keyboard_globals.buffer_size = 0;
        virtual_keyboard_globals.last_event = -1;
        virtual_keyboard_globals.keyboard = TAG_GET(virtual_keyboard, keyboard_tag);
        virtual_keyboard_globals.last_key = -1;
        virtual_keyboard_globals.number_of_event_repeats = 0;
        virtual_keyboard_globals.text_buffer = 0;
        virtual_keyboard_globals.insertion_point = 0;
        virtual_keyboard_globals.time_of_last_event = 0;
    }

    virtual_keyboard_globals.caret_bitmap_tag = tag_loaded(0x6269746Du, "ui\\shell\\bitmaps\\white");   /* 'bitm' */
    return virtual_keyboard_globals.keyboard != 0;
}
