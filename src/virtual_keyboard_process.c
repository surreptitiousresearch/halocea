/* virtual_keyboard_process @0x83784780 — per-frame tick of the on-screen keyboard; runs the internal
 * processor only while the keyboard is active.
 * FAITHFUL QUIRK: virtual_keyboard_process_internal (confirmed via its own decompilation) returns void, so on
 * the active path this function's "return value" is really whatever r3 happens to hold when that call
 * returns; on the inactive path it's simply uninitialized. The caller (process_ui_widgets.c) feeds this
 * garbage value straight into event_manager_flush()'s controller_index argument — reproduced as-is rather
 * than "fixed", since this is genuine original-binary behavior, not a decompiler artifact. */

extern void virtual_keyboard_process_internal(void);

#include "headers/virtual_keyboard_globals_t.h"

int virtual_keyboard_process(void)
{
    int result;
    if ( virtual_keyboard_globals.active )
    {
        virtual_keyboard_process_internal();
        return result;  /* r3 undefined: holds whatever virtual_keyboard_process_internal last left there */
    }
    return result;  /* r3 undefined on this path too */
}
