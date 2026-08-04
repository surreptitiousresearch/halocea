#include "headers/virtual_keyboard_globals_t.h"
#include <stdint.h>

extern void event_manager_flush(void);

void virtual_keyboard_dispose(void) /* attested void: tail-b of void fn, funcptr-only callers */
{
    virtual_keyboard_globals.active = 0;
    virtual_keyboard_globals.symbols_active = 0;
    virtual_keyboard_globals.keyboard = 0;
    virtual_keyboard_globals.last_event = -1;
    virtual_keyboard_globals.shift_active = 0;
    virtual_keyboard_globals.caps_active = 0;
    virtual_keyboard_globals.row = 0;
    virtual_keyboard_globals.column = 0;
    virtual_keyboard_globals.buffer_size = 0;
    virtual_keyboard_globals.last_key = -1;
    virtual_keyboard_globals.number_of_event_repeats = 0;
    virtual_keyboard_globals.text_buffer = 0;
    virtual_keyboard_globals.insertion_point = 0;
    virtual_keyboard_globals.time_of_last_event = 0;
    event_manager_flush();
}
